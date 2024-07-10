/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:46:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/10 16:29:33 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc.hpp"

class Server;

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

		// Server *Serv;
	public:
		Client();
		
		void SetFd(int fd);
		int GetFd();

		void SetIp(std::string ip);

		void SetNick(std::string nickname);
		std::string GetNick();
		void NickCheck(int fd_newClient);

		void SetUsername(std::string username);
		std::string GetUsername();
		void UserCheck(int fd_newClient);

		void SetHostname(std::string hostname);
		std::string GetHostname();

		void SetRealname(std::string realname);
		std::string GetRealname();

		
};

#endif