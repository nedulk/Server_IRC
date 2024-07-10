/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:58:46 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/10 16:11:49 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <unistd.h>
# include <cstring>
# include <cstdlib>
# include <vector>
# include <netinet/in.h>
# include <string>
# include <csignal> 
# include <fcntl.h>
# include <sys/socket.h> 
# include <arpa/inet.h>
# include <poll.h> 
# include <sys/types.h>

# define RESET   "\033[0m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define BLUE    "\033[34m"
# define YELLOW  "\033[33m"

# include "Server.hpp"
# include "Client.hpp" 

#endif