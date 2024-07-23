/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:46:47 by kprigent          #+#    #+#             */
/*   Updated: 2024/07/23 13:56:46 by kprigent         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot.hpp"

int main() 
{
    Bot bot("127.0.0.1", 6667, "Bot", "Bot");
    if (bot.connectToServer())
	{
        bot.authenticate();
        bot.joinChannel("#testBot");
        bot.listen();
    }

    return (0);
}