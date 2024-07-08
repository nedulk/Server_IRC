/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 17:30:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/08 18:19:23 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/Server.hpp"

void Server::passwordHandler(int fd_newClient)
{
	char buff_s[] = "mdp: ";
	char buff_r[1024];
	this->_Password = "test";
	size_t size = this->_Password.size();
	
	
	send(fd_newClient, buff_s, 5,0);
	while (std::strncmp(buff_r, this->_Password.c_str(), size) != 0)
	{
		recv(fd_newClient, buff_r, sizeof(buff_r) - 1, 0);
		buff_r[size] = '\0';
		if (buff_r[size - 1] == '\n')
			std::cout << "coucou" << std::endl;
	}
	
	std::cout << GREEN "Client |" << fd_newClient << "| connected!" RESET << std::endl;
}
