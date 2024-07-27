/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:30:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/27 16:31:28 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int Server::NickCheck_oc(const std::string& buff_rr)
{
	if (!_Clients.empty())
	{
		for(std::vector<Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it)
		{
			if ((*it)->GetNick() == buff_rr)
				return (1);
		}
	}
	return (0);
}
void Server::PasswordCheck(int fd_newClient, const std::string& message)
{
	std::string pass = message.substr(5);
	pass.erase(std::remove(pass.begin(), pass.end(), ' '), pass.end());
	
	if (pass == _PasswordBot) 
	{
		getClientByFd(fd_newClient)->setBot();
		getClientByFd(fd_newClient)->setPassCheck(1);
	} 
	else if (pass == _Password) 
	{
		getClientByFd(fd_newClient)->setPassCheck(1);
	} 
	else
	{
		std::string error_message = ERR_PASSWDMISMATCH;
		send(fd_newClient, error_message.c_str(), error_message.size(), 0);
		std::cout << RED "Error: ERR_PASSWDMISMATCH [" << fd_newClient << "]" RESET << std::endl;
	}
}

void Server::NickCheck(int fd_newClient, Client* newClient, const std::string& message, int mode)
{
	std::string nick = message.substr(5);
	nick.erase(std::remove(nick.begin(), nick.end(), ' '), nick.end());

	if (NickCheck_oc(nick) == 0) 
	{
		regex_t regex;
		int ret = regcomp(&regex, NICK , REG_EXTENDED);
		if (!ret)
		{
			ret = regexec(&regex, nick.c_str(), 0, NULL, 0);
			if ((!ret && !newClient->getBot() && nick != "Bot") || (!ret && newClient->getBot() && nick == "Bot"))
			{
				newClient->SetNick(nick);
				if (mode == 0)
					getClientByFd(fd_newClient)->setNickCheck(1);
			} 
			else 
			{
				std::string error_message = ERR_ERRONEUSNICKNAME(nick) + "\r\n";
				send(fd_newClient, error_message.c_str(), error_message.size(), 0);
				std::cout << RED "Error: ERR_ERRONEUSNICKNAME " << "[" << newClient->GetIp() << "] ["
					<< newClient->GetFd() << "]" RESET << std::endl;
			}
		}
		regfree(&regex);
	} 
	else 
	{
		std::string error_message = ERR_NICKNAMEINUSE(nick) + "\r\n";
		send(fd_newClient, error_message.c_str(), error_message.size(), 0);
		std::cout << RED "Error: ERR_NICKNAMEINUSE " << "[" << newClient->GetIp() << "] ["
			<< newClient->GetFd() << "]" RESET << std::endl;
	}
}

 void Server::UserCheck(int fd_newClient, Client* newClient, const std::string& message) 
 {
	std::string messageReg = message.substr(5);
	std::istringstream iss(message.substr(5));
	std::string username, realname, temp;

	regex_t regex;
	int ret = regcomp(&regex, USER, REG_EXTENDED);
	if (!ret)
	{
		ret = regexec(&regex, messageReg.c_str(), 0, NULL, 0);
		if (!ret) 
		{
			std::getline(iss, username, ' ');

			while (std::getline(iss, temp, ':'))
			{
				if (!temp.empty())
				{
					realname = temp;
					break ;
				}
			}
			newClient->SetUsername(username);
            newClient->SetRealname(realname);
			getClientByFd(fd_newClient)->setUserCheck(1);
		}
	}
	regfree(&regex);
}

void Server::handle_message(int fd_newClient, Client* newClient) 
{
	while (!message_queue.empty())
	{
		std::string message = message_queue.front();
		message_queue.pop();

		if (message.find("PASS ") == 0 && getClientByFd(fd_newClient)->getPassCheck() == 0)
		{
			PasswordCheck(fd_newClient, message);
			if (getClientByFd(fd_newClient)->getPassCheck() == 1)
			{	
				std::cout << "PASS cmd [OK] [" << fd_newClient << "]" << std::endl;
				getClientByFd(fd_newClient)->setPassCheck(2);
			}
		} 
		else if (message.find("NICK ") == 0 && getClientByFd(fd_newClient)->getNickCheck() == 0)
		{
			NickCheck(fd_newClient, newClient, message, 0);
			if (getClientByFd(fd_newClient)->getNickCheck() == 1)
			{	
				std::cout << "NICK cmd [OK] [" << fd_newClient << "]" << std::endl;
				getClientByFd(fd_newClient)->setNickCheck(2);
			}	
		} 
		else if (message.find("USER ") == 0 && getClientByFd(fd_newClient)->getUserCheck() == 0) 
		{
			UserCheck(fd_newClient, newClient, message);
			if (getClientByFd(fd_newClient)->getUserCheck() == 1)
			{
				std::cout << "NICK cmd [OK] [" << fd_newClient << "]" << std::endl;
				getClientByFd(fd_newClient)->setUserCheck(2);
			}
		}
	}
}
	
void Server::receive_message(int fd_newClient, Client* newClient)
{
	char buff_r[1024];
	std::memset(buff_r, 0, sizeof(buff_r));

	size_t bytes = recv(fd_newClient, buff_r, sizeof(buff_r) - 1, 0);
	if (bytes <= 0)
	{
		std::cout << ITALIC "Client [" << getClientByFd(fd_newClient)->GetIp() << "]" << " [" << fd_newClient << "]" RESET;
			std::cout << BRED " disconnected ×" RESET << std::endl;
		close(fd_newClient);
		return;
	}

	std::string message(buff_r);
	Client::splitAndKeepLastTwo(message);

	for (std::vector<std::string>::iterator it = Client::remain_line.begin(); it != Client::remain_line.end(); ++it)
	{
		message_queue.push(*it);
	}

	handle_message(fd_newClient, newClient);

	Client::remain_line.clear();
}

void Server::FirstCoHandler(int fd_newClient, Client* newClient)
{
	receive_message(fd_newClient, newClient);

	if (all_check_ok(fd_newClient))
	{
		if (newClient->getBot() == false)
		{	
			std::cout << ITALIC "New client [" << newClient->GetIp() << "]" << " [" << newClient->GetFd() << "]" RESET;
			std::cout << BGREEN " connected ✔️" RESET << std::endl;
		}
		else
		{
			std::cout << ITALIC "Bot [" << newClient->GetIp() << "]" << " [" << newClient->GetFd() << "]" RESET;
			std::cout << BGREEN " connected ✔️" RESET << std::endl;
		}

		//WELCOME MSG -> to client 
		std::string message = RPL_WELCOME(newClient->GetNick(), newClient->GetUsername(), Command::getHostname()) + "\n"
			+ RPL_YOURHOST("RCI", "1.2.5") + "\n" + RPL_CREATED("today") + "\n" + RPL_MYINFO("RCI", "1.2.5", "i/t/k/o/l", "...") + "\n";
		send(fd_newClient, message.c_str(), message.size(), 0);
		std::cout << GREEN "Reply: RPL_WELCOME / RPL_YOURHOST / RPL_CREATED / RPL_MYINFO " << "[" << fd_newClient << "]" RESET << std::endl;
	}
}

bool Server::all_check_ok(int fd_newClient)
{
	if (getClientByFd(fd_newClient)->getPassCheck() == 2 && getClientByFd(fd_newClient)->getNickCheck() == 2 
			&& getClientByFd(fd_newClient)->getUserCheck() == 2)
		return (true);
	return (false);
}
	
std::map<std::string, Channel*> Server::getChannelList()
{
	return (_channelList);
}

void Server::deleteChannel(std::string &channelName)
{
	_channelList.erase(channelName);
}

void Server::createChannel(Client *oper, std::string &channelName, std::string key)
{
	try
	{
		if (channelName == "#PingBot" && oper->getBot() == false)
			throw(std::runtime_error("Error: A client cannot create this Channel due to restrictions."));
		Channel	*newChannel = new Channel(channelName);

		if (!key.empty())
		{
			newChannel->setKey(key);
			newChannel->setIsChannelKey(true);
		}
		newChannel->addOperator(oper);
		newChannel->addUser(oper);
		_channelList[channelName] = newChannel;
		oper->joinedChannel(newChannel);
		std::cout << "Channel " + channelName + " successfully created" << std::endl;
		std::cout << "User " + oper->GetNick() + " successfully joined" << std::endl;
		newChannel->broadcastMsg(":" + oper->GetNick() + "!" + oper->GetUsername() + "@" +
			Command::getHostname() + " JOIN " + channelName + " * :" + oper->GetRealname() + "\r\n",
			*oper, true);
		newChannel->broadcastUserList(*oper);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		oper->sendErrorMsg(":ircserv 403 " + oper->GetNick() + " " + channelName + " :No such channel\r\n");
	}
}
