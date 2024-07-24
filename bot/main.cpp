/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:46:47 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/23 18:12:23 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

int main(int argc, char **argv) 
{
    if (argc == 2)
    {
        Bot bot("127.0.0.1", 6667, "Bot", "Bot", argv[1]);
        if (bot.connectToServer())
        {
            bot.authenticate();
            bot.joinChannel("#test");
            bot.listen();
        }
        return (0);
    }
    std::cout << "One argument needed (password)" << std::endl;
}