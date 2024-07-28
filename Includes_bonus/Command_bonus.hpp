/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command_bonus.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:42:32 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/28 10:02:53 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "irc_bonus.hpp"

class Command
{
	private:
		static void						limitFlag(bool input, Channel* channel, Client& client, std::string limit);
		static void						operatorFlag(bool input, Server& server, Channel* channel, Client& client, std::string user);
		static void						keyFlag(bool input, Channel* channel, Client& client, std::string key);
		static void						topicFlag(bool input, Channel* channel, Client& client);
		static void						inviteFlag(bool input, Channel* channel, Client& client);
		static std::vector<std::string>	getJoinChannels(std::string &arg);
		static std::vector<std::string>	getJoinKeys(std::string &arg);
		static void						joinChannel(Server& server, Client& sender, std::string &channelName, std::string key);
		static void	executeModes(Server& server, Client& client, Channel* channel, std::pair<int, std::string>& flags,
								std::vector<std::string>& cmdArgs);
		static void callModes(Server& server, Client& client, Channel* channel, std::string flag, std::string& arg);
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
		static void	modeCmd(Server& server, Client& client, std::vector<std::string> args);
};

#endif