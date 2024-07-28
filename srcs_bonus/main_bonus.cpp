/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:11:46 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/28 10:06:30 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc_bonus.hpp"

int main(int argc, char **argv)
{
	if (argc != 4)
		std::cout << RED "Missing arguments" RESET << std::endl;
	else
	{
		int port = std::atoi(argv[1]);
		std::string password1 = argv[2];
		std::string password2 = argv[3];

		if (port <= 0 || port > 65535)
		{
			std::cout << RED "Invalid port number" RESET << std::endl;
			return (1);
		}

		if (password1.length() < 3 || password2.length() < 3 || password1.find(' ') != std::string::npos
			|| password2.find(' ') != std::string::npos)
		{
			std::cout << RED "Passwords must be at least 3 characters long without spaces" RESET << std::endl;
			return (1);
		}

		Server server(port, password1.c_str(), password2.c_str());
		std::cout << YELLOW << "===================== SERVER IRC =====================" << RESET << std::endl;
		try
		{
			signal(SIGINT, Server::SignalHandler);
			signal(SIGQUIT, Server::SignalHandler);
			server.ServerInit();
		}
		catch(const std::exception& e)
		{
			server.CloseFds();
			std::cerr << e.what() << std::endl;
		}
	}
}