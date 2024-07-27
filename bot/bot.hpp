/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:34:26 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/27 15:01:51 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <algorithm>
# include <string>
# include <cstring>
# include <vector>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <poll.h>
# include <cctype>

class Bot 
{
	private:
		int			_sockfd;
		std::string _server_ip;
		int			_port;
		std::string _nickname;
		std::string _username;
		std::string _pass;
		static bool _Signal;

	public:
		
		Bot(const std::string& server_ip, int port, const std::string& nickname, const std::string& username, const std::string pass);
		bool connectToServer();
		void sendRaw(const std::string& message);
		void authenticate();
		void joinChannel(const std::string& channel);
		void listen();
		~Bot();

		std::string trueMessage(std::string message);
		static void SignalHandler(int signum);
};

#endif
