/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_bonus.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:46:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/28 10:05:39 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "irc_bonus.hpp"

class Server;
class Channel;

class Client
{
	private:
		int _fd;
		std::string _IP;

		//NICK
		std::string _Nickname;
		//USER
		std::string _Username;
		std::string _Hostname;
		std::string _Realname;

		int user_check;
		int nick_check;
		int pass_check;

		std::vector<Channel *> _joinedChannels;
		bool _bot;

	public:
		Client();
		~Client();
		
		int getUserCheck();
		int getPassCheck();
		int getNickCheck();
		void setNickCheck(int i);
		void setUserCheck(int i);
		void setPassCheck(int i);

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

		static void splitAndKeepLastTwo(const std::string& str);
		static std::string compare_remain_line(const std::string& str);
		static std::vector<std::string> remain_line;
		void	sendErrorMsg(std::string msg);
};

#endif