/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:46:47 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/28 12:38:12 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

int main(int argc, char **argv) 
{
    signal(SIGINT, Bot::SignalHandler);
    if (argc == 3)
    {
        Bot bot("127.0.0.1", atoi(argv[2]), "Bot", "Bot", argv[1]);
        if (bot.connectToServer())
        {
            bot.authenticate();
            bot.joinChannel("#PingBot");
            if (bot.listen() == 1)
                return (1);
        }
        return (0);
    }
    std::cout << "Two arguments needed [First]->password [Second]->port" << std::endl;
}