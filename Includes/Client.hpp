/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 16:46:21 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/07 16:58:37 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include <string>

class Client
{
	private:
		int _fd; // client fd
		std::string _IP; // Adresse Ip du client 
	public:
		Client() {}
		
		void SetFd(int fd): _fd(fd) {}
		int GetFd() {return _fd;}

		void SetIp(std::string ip): _IP(ip) {}
};

#endif