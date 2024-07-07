/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 15:49:32 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/07 16:58:00 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include "Client.hpp"

// La class Server possede toutes les informations du serveur.

class Server
{
	private:
		int _Port; // "ex: port 80" . Point d'acces numerique sur le pc. Permet de differencier les services.
		int _ServerSocketFd; // Utilisation de la fonction socket() qui va return un fd
							 // Le socket va etre le point final de communication. "Ex: 12.15.45.34:80"
		
		std::vector<Client> _Clients; // Ici on va stocker la liste des Clients qui sont connectes sur le serveur.
		std::vector<struct pollfd> fds; // Ici on va stocker tous les fds des Clients, pour "surveiller".
	
		static bool _Signal;	
	public:
		Server(): _ServerSocketFd(-1) {} // -1 cela permet d'indiquer que le socket n'est pas encore initialise/pas
										 // encore ouvert.
		void ServeurInit();
		void ServeurSocket();
		void AcceptNewClient();
		void ReceiveNewData(int fd);

		static void SignalHandler(int signum); // La static permet d'acceder a la variable static _Signal sans instancier 
											    // un objet Server.
		void CloseFds();
		void ClearClients(int fd);
};
		
#endif		
		