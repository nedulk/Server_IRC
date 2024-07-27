/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:49:32 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/27 14:58:46 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

class Client;
class Channel;
class Command;

class Server
{
	protected:
		
		std::string		_Password;
		std::string		_PasswordBot;
		int 			_Port;
		int				_ServerSocketFd;
		
		std::vector<struct pollfd>		fds;
		std::vector<Client*>			_Clients;
		std::map<std::string, Channel*>	_channelList;
		
		std::queue<std::string>		message_queue;
		Client 						*Cli;
		static bool					_Signal;
		
	public:
		
		Server(int port, std::string password, std::string passwordBot);
		~Server();
		
		void	ServerInit();
		void	ServerSocket();
		void	NewClient();
		void	ReceiveNewData(int fd);

		static void	SignalHandler(int signum);
		void 		FirstCoHandler(int fd_newClient, Client *newClient);
		
		int		NickCheck_oc(const std::string& buff_rr);
		void	NickCheck(int fd_newClient, Client* newClient, const std::string& message);
		void	UserCheck(int fd_newClient, Client* newClient, const std::string& message) ;
		void	PasswordCheck(int fd_newClient, const std::string& message);
		
		bool all_check_ok(int fd_newClient);
		void receive_message(int fd_newClient, Client* newClient);
		void handle_message(int fd_newClient, Client* newClient);
		
		void CloseFds();
		void ClearClients(int fd);
		void ClearAllClients();
		void ClearAllChannels();
		void printState();

		Client*	getClientByName(std::string name, int mode);
		Client*	getClientByFd(int fd);
		std::vector<Client *>	getClients();
		std::map<std::string, Channel*>	getChannelList();

		void	deleteChannel(std::string& channelName);
		void	createChannel(Client* oper, std::string& channelName, std::string key);
};
		
#endif		
		