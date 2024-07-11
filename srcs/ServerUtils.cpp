/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:30:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/10 17:11:57 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::PasswordCheck(int fd_newClient)
{
	char buff_r[1024];
	// char *stock_addr = buff_r; 
	for (int i = 0; i < 1024; i++)
		buff_r[i] = '\0';
	
	while (1)
	{
		if (Server::_Signal == true)
			exit(0);
		recv(fd_newClient, buff_r, sizeof(buff_r) - 1, 0);
		if (std::strncmp(buff_r, "PASS ", 5) == 0)
		{
			char *buff_rr = buff_r + 5;
			while (*buff_rr == ' ')
				buff_rr++;
			char *p = buff_rr;
			while (*p != '\0')
			{
				if (*p == '\n')
				{
					*p = '\0';
					break;
				}
				p++;
			}
			if(std::strcmp(buff_rr, this->_Password.c_str()) == 0)
				break;
		}
	}	
}

void Server::FirstCoHandler(int fd_newClient, Client *newClient)
{
	//PASS
	PasswordCheck(fd_newClient);
	std::cout << "Password [OK]" << std::endl;
	
	//NICK
	newClient->NickCheck(fd_newClient);
	std::cout << "Nickname [OK]" << std::endl;

	//USER
	newClient->UserCheck(fd_newClient);
	std::cout << "Username [OK]" << std::endl;
	
	std::cout << GREEN "Client |" << fd_newClient << "| connected!" RESET << std::endl;
	
	std::cout << GREEN "Nickname: " << newClient->GetNick() << "" RESET << std::endl;

	std::cout << GREEN "Username: " << newClient->GetUsername() << "" RESET << std::endl;
}
