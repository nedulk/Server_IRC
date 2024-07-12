/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 17:00:24 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/12 11:56:57 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"


// informations de l'adresse
// -> sin_family: entier qui represente la famille de l'adress. IPv4, IPv6 ...
//                 Pour IPv4 AF_INET.
// -> sin_port: entier sur 16 bits qui represente le numero de port.
// -> sin_addr: struct qui contient l'adress IPv4. INADDR_ANY represente l'ip "any"
//              ce qui signifie que le socket sera lie a toues les interfaces reseau disponibles
//              sur l'hote. Ex: carte reseau, wifi... chaque interface a une address unique; l'adresse MAC
// -> sin_zero: rembourage pour rendre la struct de mm taille que sckaddr pour des raisons de compatibilite.
//				rempli de zeros.
// 
// struct sockaddr_in 
// {
//  	sa_family_t     sin_family;
//  	in_port_t       sin_port;
//  	struct  in_addr sin_addr;
//  	char            sin_zero[8];
// };

// Cette structure est utilisee pour surveiller les descripteurs de fichier pour les entrees et sorties (I/O)
// Utilisee avec poll() en general.

// struct pollfd 
// {
// 	int     fd; //-> file descriptor
//  	short   events; //-> requested events
//  	short   revents; //-> returned events
// };

// struct in_addr
// {
//  	in_addr_t s_addr;
// };

bool Server::_Signal = false;

void Server::SignalHandler(int signum)
{
	(void)signum;
	Server::_Signal = true; // stop serv
}

// -1 permet d'indiquer que le socket n'est pas encore initialise/pas encore ouvert.
Server::Server(int port, std::string password): _Password(password), _Port(port), _ServerSocketFd(-1)
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
		if((poll(&fds[0],fds.size(),-1) == -1) && Server::_Signal == false) //permet d'attendre un event
			throw(std::runtime_error("Poll() failed"));
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == this->_ServerSocketFd)
				{	
					// -> le serveur a un socket d'ecoute et attend une requete sur port specifique (ici 6667)
					// -> le serveur recoit une demande sur ce socket d'ecoute
					// -> le serveur accepte la connexion
					// -> le serveur cree un socket unique pour la communication 
					NewClient(); // on va accepter le nouveau client et cree un socket de communication entre le serveur et ce client specifique
				}
				else		
					ReceiveNewData(fds[i].fd);
			}
		}
	}
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
	FirstCoHandler(socketFd_newClient, newClient);
	_Clients.push_back(newClient);
}

void Server::ReceiveNewData(int fd)
{
	char buff[1024];
	for (int i = 0; i < 1024; i++)
		buff[i] = 0;
	
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0); // reception de la data

	if (bytes <= 0) // le client est deco
	{
		std::cout << RED "Client |" << fd << "| disconnected" RESET << std::endl;
		ClearClients(fd);
		close(fd);
	}
	else
	{
		buff[bytes] = '\0';
		std::string tmpStr = buff;
		if (tmpStr == "privmsgtest1")
		{
			std::string name = "test";
			std::string msg = "bla";
			Command::privMsg(*this, getClientByFd(fd), name, msg);
		}
		if (tmpStr == "privmsgtest2")
		{
			std::string name = "#chan1";
			std::string msg = "message on chan1";
			Command::privMsg(*this, getClientByFd(fd), name, msg);
		}
		if (tmpStr == "jointest1")
		{
			std::string	channelName = "#chan1";
			std::string	channelKey = "key";
			Command::join(*this, getClientByFd(fd), channelName, channelKey);
		}
		if (tmpStr == "jointest2")
		{
			std::string	channelName = "#chan1";
			std::string	channelKey = "key";
			Command::join(*this, getClientByFd(fd), channelName, channelKey);
		}
		std::cout << BLUE "Client |" << fd << "| : " RESET << buff;
	}
}

void Server::ClearClients(int fd)
{
	// Supression d'un client dans la liste. Ex: deconnexion
	for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it) // pre-incrementation par convention
	{																		  // ++it incremente la valeur et retourne 			
		if ((*it)->GetFd() == fd)														     // la valeur incrementee
		{
			_Clients.erase(it);
			break ;
		}
	}

	for (std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); ++it)
	{
		if (it->fd == fd)
		{
			fds.erase(it);
			break ;
		}
	}
}

void Server::CloseFds()
{
	std::cout <<  YELLOW "Disconnecting all clients ..." RESET << std::endl;
	for (size_t i = 0; i < _Clients.size() ; i++)
	{
		close(_Clients[i]->GetFd());
	}
	if (this->_ServerSocketFd != -1)
	{
		std::cout << YELLOW "Terminating server: Closing the listening socket..." RESET << std::endl;
		close(this->_ServerSocketFd);
	}
}

std::string getCurrentTime()
{
	std::time_t result = std::time(0);
	std::string timeStr(ctime(&result));
	return timeStr.substr(0, timeStr.size()-1);
}

void Server::printState()
{
	std::cout << std::endl << BLACK << getCurrentTime() << "    " << "Clients connected: " << _Clients.size() << std::endl;
	std::cout << BLACK << "                       " << "    " << GREEN << "----------------------" << RESET << std::endl;
	if (!_Clients.empty())
	{
		for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
		{
			std::cout << BLACK << "                       " << "    " << "Client nick: " << (*it)->GetNick() << std::endl;
			std::cout << BLACK << "                       " << "    " << "Client user: " << (*it)->GetUsername() << std::endl;
			std::cout << BLACK << "                       "<< "    " << "Client fd: " << (*it)->GetFd() << std::endl;
			if (it != _Clients.end() - 1)
				std::cout << BLACK << "                           " << GREEN << "-----------" << RESET << std::endl;
		}
	}
	// std::cout << BLACK << getCurrentTime() << "    " << YELLOW << "------------------------" << RESET << std::endl;
	// std::cout << BLACK << getCurrentTime() << "    " << "Channels: " << _Channels.size() << std::endl;
	// std::cout << BLACK << getCurrentTime() << "    " << GREEN << "-----------" << RESET << std::endl;
    // if (_channels.size() > 0)
    // {
    //     for (size_t i = 0; i < _channels.size(); i++)
    //     {
    //         std::cout << BLACK << getCurrentTime() << "    " << "Channel name: " << _channels[i]->getName() << std::endl;
    //         std::cout << BLACK << getCurrentTime() << "    " << "Channel topic: " << _channels[i]->getTopic() << std::endl;
    //         for (size_t i = 0; i < getChannels().size(); i++)
    //         {
    //             std::vector<Client *> lst = getChannels()[i]->getUserList();
    //             for (size_t j = 0; j != lst.size(); j++)
    //             {
    //                 std::cout << BLACK << getCurrentTime() << "    " << "Client nick: " << lst[j]->getNick() << std::endl;
    //             }
    //         }
    //         if (i < _channels.size() - 1)
    //             std::cout << BLACK << getCurrentTime() << "    " << GREEN << "-----------" << RESET << std::endl;
    //     }
    // }
}

Client *Server::getClientByName(std::string name)
{
	for (std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); it++)
	{
		if ((*it)->GetNick() == name)
			return (*it);
	}
	throw (std::runtime_error("Client not found"));
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

void Server::broadcastMsg(std::string& msg, std::string& channelName, Client& sender)
{
	try
	{
		Channel	*channel = _channelList.at(channelName);
		if (channel->getUserList().count(sender.GetFd()) == 0)
		{
			std::cerr << RED << sender.GetNick() << " not in channel " << channelName << RESET << std::endl;
			return ;
		}

		std::map<int, Client*> userList = channel->getUserList();
		for (std::map<int, Client*>::iterator it = userList.begin();
			it != userList.end();
			it++)
		{
			if (it->second->GetFd() != sender.GetFd())
				send(it->second->GetFd(), msg.c_str(), msg.size(), 0);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << RED "Channel '" << channelName <<  "' doesn't exist" RESET << std::endl;
	}
}
