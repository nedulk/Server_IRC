/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:49:32 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/08 17:52:22 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP


# include <iostream>
# include <unistd.h>
# include <cstring>
# include <vector>
# include <netinet/in.h>
# include <csignal> 
# include <fcntl.h>
# include <sys/socket.h> 
# include <arpa/inet.h>
# include <poll.h> 
# include <sys/types.h>
# include "Client.hpp"

# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define BLUE    "\033[34m"
# define YELLOW  "\033[33m"

// La class Server possede toutes les informations du serveur.

class Server
{
	private:
		
		std::string _Password;
		int _Port; // "ex: port 80" . Point d'acces numerique sur le pc. Permet de differencier les services.
		int _ServerSocketFd; // Utilisation de la fonction socket() qui va return un fd
							 // Le socket va etre le point final de communication. "Ex: 12.15.45.34:80"
		
		std::vector<Client> _Clients; // Ici on va stocker la liste des Clients qui sont connectes sur le serveur.
		std::vector<struct pollfd> fds; // Ici on va stocker tous les fds des Clients, pour "surveiller".
	
		static bool _Signal;	
	public:
		Server();
		void ServerInit();
		void ServerSocket();
		void NewClient();
		void ReceiveNewData(int fd);

		static void SignalHandler(int signum); // La static permet d'acceder a la variable static _Signal sans instancier 
											    // un objet Server.
		void passwordHandler(int fd_newClient);
		void CloseFds();
		void ClearClients(int fd);
};
		
#endif		
		