/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 11:42:32 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/10 11:43:47 by kprigent         ###   ########.fr       */
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
	static void	execCmd(Server& server, int cmdName, Client& sender, std::vector<std::string> args);
};

#endif