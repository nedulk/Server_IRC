/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:42:32 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/14 17:18:02 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "irc.hpp"

class Command
{
	private:
	public:
		
		Server *server;
		
		/////CMD handling /////	
		static std::string RegexCmd(std::string buff);
		static std::vector<std::string> GetCmdArgs(std::string buff);
		void execCmd(Server& server, Client& client, std::string cmdName, std::vector<std::string> args);
		
		/////CMD fct ///////
		static void	join(Server& server, Client* sender, std::string& channel, std::string key);
		static void	privMsg(Server& server, Client& client, std::vector<std::string> args);
		static void quitCmd(Server& server, Client& client, std::vector<std::string> args);
};

#endif