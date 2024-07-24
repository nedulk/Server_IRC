/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:42:32 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/23 17:39:44 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "irc.hpp"

class Command
{
	private:
		static std::vector<std::string>	getJoinChannels(std::string &arg);
		static std::vector<std::string>	getJoinKeys(std::string &arg);
		static void						joinChannel(Server& server, Client& sender, std::string &channelName, std::string key);
	public:

		Server *server;
	/////CMD handling /////
		static std::string RegexCmd(std::string buff);
		static std::vector<std::string> GetCmdArgs(std::string buff);
		static void execCmd(Server& server, Client& client, std::string cmdName, std::vector<std::string> args);

		static std::string				getHostname();

		/////CMD fct ///////
		static void	joinCmd(Server& server, Client& sender, std::vector<std::string> &args);
		static void	privMsg(Server& server, Client& client, std::vector<std::string> args);
		static void quitCmd(Server& server, Client& client, std::vector<std::string> args);
		static void	kickCmd(Server& server, Client& client, std::vector<std::string> args);
		static void	partCmd(Server& server, Client& client, std::vector<std::string> args);
		static void	whoCmd(Server& server, Client& client, std::vector<std::string> args);
		static void	inviteCmd(Server& server, Client& client, std::vector<std::string> args);
		static void	topicCmd(Server& server, Client& client, std::vector<std::string> args);
		// static void	modeCmd(Server& server, Client& client, std::vector<std::string> args);
};

#endif