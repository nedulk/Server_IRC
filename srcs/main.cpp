/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 16:11:46 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/11 20:13:50 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

//////////////// MONITORING DEBUG ////////
#include <thread> 						//
#include <atomic>						//
#include <mutex>						//	
std::atomic<bool> running(true);		//
std::mutex cout_mutex;					//
//////////////////////////////////////////

int main(int argc, char **argv)
{
	if (argc != 3)
		std::cout << RED "Missing arguments" RESET << std::endl;
	else
	{
		///////////////////////// MONITORING DEBUG ////////////////////////////////////////////
		Server server(std::atoi(argv[1]), argv[2]);
		std::thread serverStateThread([&server]()
		{
			while (running)
			{
				cout_mutex.lock();
				std::cout << "\033[2J\033[H";
				std::cout << YELLOW << "================= SERVER STATE =================" << RESET << std::endl;
				server.printState();
				cout_mutex.unlock();
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		});
		serverStateThread.detach();
		//////////////////////////////////////////////////////////////////////////////////////
		
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