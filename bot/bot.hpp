/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:34:26 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/23 15:43:13 by kprigent         ###   ########.fr       */
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
		int sockfd;
		std::string server_ip;
		int port;
		std::string nickname;
		std::string username;

	public:
		
		Bot(const std::string& server_ip, int port, const std::string& nickname, const std::string& username);
		bool connectToServer();
		void sendRaw(const std::string& message);
		void authenticate();
		void joinChannel(const std::string& channel);
		void listen();
		~Bot();

		std::string trueMessage(std::string message);
};

#endif
