/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:11:46 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/24 17:00:56 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(int argc, char **argv)
{
	if (argc != 4)
		std::cout << RED "Missing arguments" RESET << std::endl;
	else
	{
		Server server(std::atoi(argv[1]), argv[2], argv[3]);
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