/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:46:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/24 16:42:17 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Server;
class Channel;

class Client
{
	private:
		int _fd; // client fd
		std::string _IP; // Adresse Ip du client

		//NICK
		std::string _Nickname;
		//USER
		std::string _Username;
		std::string _Hostname;
		std::string _Realname;

		std::vector<Channel *> _joinedChannels;
		bool _bot;

	public:
		Client();
		~Client();
		
		bool getBot();
		void setBot();
		
		void SetFd(int fd);
		int GetFd();

		void SetIp(std::string ip);
		std::string GetIp();

		void SetNick(std::string nickname);
		std::string GetNick();
		

		void SetUsername(std::string username);
		std::string GetUsername();

		void SetHostname(std::string hostname);
		std::string GetHostname();

		void SetRealname(std::string realname);
		std::string GetRealname();

		void	joinedChannel(Channel *channel);
		void	leftChannel(Channel *channel);
		std::vector<Channel *>	getJoinedChannels();

		void	sendErrorMsg(std::string msg);
};

#endif