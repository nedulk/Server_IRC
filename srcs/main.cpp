/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:11:46 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/10 16:11:40 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/irc.hpp"

int main(int argc, char **argv)
{
	if (argc != 3)
		std::cout << RED "Missing arguments" RESET << std::endl;
	else
	{
		// 0 -> 1023 port reserves (ex: http (80), SSh (22)..)
		// 1024 -> 49151 plage utilisable
		Server server(std::atoi(argv[1]), argv[2]);
		std::cout << GREEN "/////////////////////   IRC SERVER   ///////////////////" RESET << std::endl << std::endl;
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