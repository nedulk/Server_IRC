/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:58:46 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/11 20:46:53 by kprigent         ###   ########.fr       */
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
# include <regex.h>
# include <ctime> // Fournit des fonctions pour manipuler les dates et les heures

# define ITALIC "\033[3m"
# define UNDERLINE "\033[4m"
# define BLINK "\033[5m"
# define INVERT "\033[7m"
# define HIDDEN "\033[8m"
# define STRIKE "\033[9m"
# define DEFAULT "\033[0m"
# define LIGHT "\033[1m"
# define DARK "\033[2m"
# define BOLD "\033[1m"
# define BLACK "\033[0;90m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define WHITE "\033[0;37m"
# define RESET "\033[0m"
# define BBLACK "\033[1;30m"
# define BRED "\033[1;31m"
# define BGREEN "\033[1;32m"
# define BYELLOW "\033[1;33m"
# define BBLUE "\033[1;34m"
# define BMAGENTA "\033[1;35m"
# define BCYAN "\033[1;36m"
# define BWHITE "\033[1;37m"
# define BRESET "\033[1;0m"

/////////////////////////////////// ERROR MSG //////////////////////////////////////////////////////////
# define ERR_NOSUCHNICK(input, nick)			"401 " + input + " " + nick + " :No such nick/channel"
# define ERR_NOSUCHCHANNEL(input, chan)			"403 " + input + " " + chan + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(input, chan)		"404 " + input + " " + chan + " :Cannot send to channel"
# define ERR_NOORIGIN(input)					"409 " + input + " :No origin specified"
# define ERR_NORECIPIENT(input)					"411 " + input + " :No recipient given"
# define ERR_NOTEXTTOSEND(input)				"412 " + input + " :No text to send"
# define ERR_UNKNOWNCOMMAND(input, cmd)			"421 " + input + " " + cmd + " :Unknown command"
# define ERR_NONICKNAMEGIVEN(input)				"431 " + input + " :No nickname given"
# define ERR_ERRONEUSNICKNAME(nick)				"432 " + nick + " :Erroneous nickname"
# define ERR_NICKNAMEINUSE(nick)				"433 * " + nick + " :Nickname is already in use"
# define ERR_USERNOTINCHANNEL(input, nick, chn)	"441 " + input + " " + nick + " " + chn + " :They aren't on that channel"
# define ERR_NOTONCHANNEL(input, chan)			"442 " + input + " " + chan + " :You're not on that channel"
# define ERR_USERONCHANNEL(input, nick, chan)	"443 " + input + " " + nick + " " + chan + " :is already on channel"
# define ERR_NOTREGISTERED(input)				"451 " + input + " :You have not registered"
# define ERR_NEEDMOREPARAMS(input, cmd)			"461 " + input + " " + cmd + " :Not enough parameters"
# define ERR_ALREADYREGISTRED(input)			"462 " + input + " :Unauthorized command (already registered)"
# define ERR_PASSWDMISMATCH(input)				"464 " + input + " :Password incorrect"
# define ERR_CHANNELISFULL(input, chan)			"471 " + input + " " + chan + " :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE(input, mode)			"472 " + input + " " + mode + " :is unknown mode char to me"
# define ERR_INVITEONLYCHAN(input, chan)		"473 " + input + " " + chan + " :Cannot join channel (+i)"
# define ERR_BADCHANNELKEY(input, chan)			"475 " + input + " " + chan + " :Cannot join channel (+k)"
# define ERR_NOPRIVILEGES(input)				"481 " + input + " :Permission Denied- You're not an IRC operator"
# define ERR_CHANOPRIVSNEEDED(input, chan)	

# define REGEXNICKNAME "^[a-zA-Z][a-zA-Z0-9_.-]{0,8}$"

# include "Server.hpp"
# include "Client.hpp" 

#endif