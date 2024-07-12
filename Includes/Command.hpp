/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:42:32 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/12 16:10:35 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "irc.hpp"

class Command
{
	private:
		static void	privMsg(Server& server, Client& sender, std::string& receiver, std::string& msg);
		static void	join(Server& server, Client& sender, std::string& channel, std::string& key);

	public:
		static std::string RegexCmd(std::string buff);
		// static void	execCmd(Server& server, std::string cmd, Client& sender, std::vector<std::string> args);
		static void	execCmd(std::string cmd);
};

#endif