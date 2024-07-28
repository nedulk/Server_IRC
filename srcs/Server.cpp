/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:00:24 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/28 09:44:24 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool Server::_Signal = false;

void Server::SignalHandler(int signum)
{
	(void)signum;
	Server::_Signal = true;
}

Server::Server(int port, std::string password): _Password(password), _Port(port), _ServerSocketFd(-1)
{
}

Server::~Server()
{
}

void Server::ServerInit()
{
	ServerSocket();

	std::cout << DARK "    Port: " << this->_Port << "  |  " RESET;
	std::cout << DARK "Socket fd: " << this->_ServerSocketFd << "  |  " RESET;
	std::cout << DARK "Password: " << this->_Password << RESET << std::endl;

	std::cout << std::endl;
	std::cout << GREEN "Waiting for clients ...." RESET << std::endl;
	
	while (Server::_Signal == false)
	{
		if((poll(&fds[0],fds.size(),-1) == -1) && Server::_Signal == false)
			throw(std::runtime_error("Poll() failed"));
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == this->_ServerSocketFd)
					NewClient();
				else		
					ReceiveNewData(fds[i].fd);
			}
		}
	}
	ClearAllChannels();
	ClearAllClients();
	CloseFds();
}

void Server::ServerSocket()
{
	struct sockaddr_in add;
	struct pollfd NewPoll;
	
	add.sin_family = AF_INET; //IPv4
	add.sin_port = htons(this->_Port); // htons garantit que le numero de port est dans le bon format
									   // pour le reseau.
	add.sin_addr.s_addr = INADDR_ANY;

	this->_ServerSocketFd = socket(AF_INET,SOCK_STREAM, 0); // (1) dommaine de communication / famille du socket (IPv4, IPv6)
													  		// (2) type de communication, ici communication bidirectionnelle en flux d'octets.
													  		// (3) protocol a utiliser avec le socket, la pluspart du temps set a 0.
	if (this->_ServerSocketFd == -1)
		throw (std::runtime_error(RED "Failed to create server socket" RESET));
	
		//////////////////////////////////////////
		////////PARAMETRAGE DU SOCKET////////////
		////////////////////////////////////////
	
	int enable = 1; // valeur que je veux definir pour l'option du socket (ici SO_REUSEADDR)
	if (setsockopt(this->_ServerSocketFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		throw(std::runtime_error(RED "Failed to set socket option: SO_REUSEADDR" RESET));
	if (fcntl(this->_ServerSocketFd, F_SETFL, O_NONBLOCK) < 0) // parametrage du fd du socket "non-blocking mode". In/out non
														       // non bloquant qui reviennent de suite sans arreter l'execution du programme
		throw(std::runtime_error(RED "Failed to set socket option: O_NONBLOCK" RESET));
	// -> utilisation de la fonction bind() pour lier un socket a une addresse
	if (bind(this->_ServerSocketFd, (struct sockaddr*)&add, sizeof(add)) < 0)
		throw (std::runtime_error(RED "Failed to bind a socket" RESET));
	if (listen(this->_ServerSocketFd, SOMAXCONN) < 0)
		throw (std::runtime_error(RED "Listen() failed"));
	
	NewPoll.fd = this->_ServerSocketFd;
	NewPoll.events = POLLIN; // reading
	NewPoll.revents = 0;

	fds.push_back(NewPoll); // ajoute du nouveau socket dans la liste
}

void Server::NewClient()
{
	Client *newClient = new Client();
	
	struct sockaddr_in newClient_addr;
	struct pollfd NewPoll;
	socklen_t len = sizeof(newClient_addr);
	
	int socketFd_newClient = accept(this->_ServerSocketFd, (sockaddr *)&(newClient_addr), &len); // accepte le client et cree le socket de communication
	if (socketFd_newClient == -1)
	{
		std::cout << RED "Accept new client failed" RESET << std::endl;
		return ;
	}
	
	if (fcntl(socketFd_newClient, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cout << RED "Failed to set socket (NewClient) option: O_NONBLOCK" RESET << std::endl;
		return ;
	}
	// pas de throw mais return car on ne veut pas stopper le serveur
	
	NewPoll.fd = socketFd_newClient;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	
	newClient->SetFd(socketFd_newClient);
	newClient->SetIp(inet_ntoa((newClient_addr.sin_addr)));
	fds.push_back(NewPoll);
	std::cout << ITALIC "New client [" << newClient->GetIp() << "]" << " [" << newClient->GetFd() << "]" RESET << std::endl;
	_Clients.push_back(newClient);
}

void Server::ReceiveNewData(int fd)
{
	char buff[1024];
	for (int i = 0; i < 1024; i++)
		buff[i] = 0;

	if (all_check_ok(fd) == false)
	{
		FirstCoHandler(fd, getClientByFd(fd));	
	}
	else
	{
		ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0);

		if (bytes <= 0)
		{
			std::cout << ITALIC "Client [" << getClientByFd(fd)->GetIp() << "]" << " [" << fd << "]" RESET;
			std::cout << BRED " disconnected ×" RESET << std::endl;
			ClearClients(fd);
			close(fd);
		}
		else
		{
			std::queue<std::string> commands;
			buff[bytes] = '\0';

			Client	*client	= getClientByFd(fd);
			client->setClientBuff(client->getClientBuff() + buff);
			if (client->getClientBuff().find("\r\n") == std::string::npos)
				return ;
			std::string message = client->getClientBuff();
			client->setClientBuff("");

			// Split message into commands
			std::string::size_type pos = 0, lastPos = 0;
			while ((pos = message.find('\n', lastPos)) != std::string::npos)
			{
				std::string command = message.substr(lastPos, pos - lastPos);

				// Remove \r from command
				command.erase(std::remove(command.begin(), command.end(), '\r'), command.end());
				if (!command.empty() && command.find_first_not_of('\n') != std::string::npos)
					commands.push(command);
				lastPos = pos + 1;
			}

			// Add last command to queue
			std::string command = message.substr(lastPos);
			command.erase(std::remove(command.begin(), command.end(), '\r'), command.end());
			if (!command.empty() && command.find_first_not_of('\n') != std::string::npos)
				commands.push(command);

			// Process each command
			while (!commands.empty())
			{
				std::string command = commands.front();
				commands.pop();

				std::string regex = Command::RegexCmd(command.c_str());
				if (!regex.empty())
					Command::execCmd(*this, *getClientByFd(fd), regex, Command::GetCmdArgs(command.c_str()));
				else
				{
					std::string errMsg = ":ircserv ";
					errMsg += ERR_UNKNOWNCOMMAND(command) + "\r\n";
					send(fd, errMsg.c_str(), errMsg.size(), 0);
					std::cout << RED "Error: ERR_UNKNOWNCOMMAND " << "[" << getClientByFd(fd)->GetIp() << "] ["
						<< fd << "]" RESET << std::endl;
				}
			}
		}
	}
}

void Server::ClearClients(int fd)
{
    // Suppression d'un client dans la liste. Ex: deconnexion
    try
    {
        Client	*client = getClientByFd(fd);
        std::vector<Channel *> joinedChannels = client->getJoinedChannels();

        for (std::vector<Channel*>::iterator it = joinedChannels.begin(); it != joinedChannels.end(); )
        {
            (*it)->delUser(client);
            if ((*it)->getOperators().count(client->GetFd()) != 0)
                (*it)->delOperator(client);
            if ((*it)->getUserCount() == 0)
            {
                deleteChannel((*it)->getName());
                delete (*it);
                it = joinedChannels.erase(it);
            }
            else
                ++it;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Unexpected problem: " << e.what() << std::endl;
    }

    std::vector<Client*>::iterator it = _Clients.begin();
    while (it != _Clients.end())
    {
        if ((*it)->GetFd() == fd)
        {
            delete *it;
            it = _Clients.erase(it);
        }
        else
            ++it;
    }

    std::vector<struct pollfd>::iterator it_fd = fds.begin();
    while (it_fd != fds.end())
    {
        if (it_fd->fd == fd)
            it_fd = fds.erase(it_fd);
        else
            ++it_fd;
    }
}

void Server::ClearAllClients()
{
    std::vector<Client*>::iterator it = _Clients.begin();
    while (it != _Clients.end())
    {
        delete *it;
        it = _Clients.erase(it);
    }
}

void Server::ClearAllChannels()
{
	for (std::map<std::string, Channel*>::iterator it = _channelList.begin(); it != _channelList.end(); ++it)
		delete it->second;
	_channelList.clear();
}

void Server::CloseFds()
{
	for (int i = 3; i < 1024 ; i++)
		close(i);
}

Client *Server::getClientByName(std::string name, int mode)
{
	for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); it++)
	{
		if ((*it)->GetNick() == name)
			return (*it);
	}
	if (mode == 0)
		throw (std::runtime_error("Client not found"));
	else
		return (NULL);
}

Client *Server::getClientByFd(int fd)
{
	for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); it++)
	{
		if ((*it)->GetFd() == fd)
			return (*it);
	}
	throw (std::runtime_error("Client not found"));
}

std::vector<Client *> Server::getClients()
{
	return (_Clients);
}
