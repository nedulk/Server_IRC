/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:11:46 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/08 16:30:08 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Client.hpp"
#include "../Includes/Server.hpp"

int main()
{
	Server server;
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