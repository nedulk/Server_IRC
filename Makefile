# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/17 12:10:05 by kprigent          #+#    #+#              #
#    Updated: 2024/07/28 10:10:41 by kprigent         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = srcs/main.cpp \
	  srcs/Client.cpp \
	  srcs/Server.cpp \
	  srcs/ServerUtils.cpp \
	  srcs/Channel.cpp \
	  srcs/Command.cpp \
	  srcs/commands/Join.cpp \
	  srcs/commands/PrivMsg.cpp \
	  srcs/commands/QuitCmd.cpp \
	  srcs/commands/Kick.cpp \
	  srcs/commands/Part.cpp \
	  srcs/commands/Who.cpp \
	  srcs/commands/Invite.cpp \
	  srcs/commands/Topic.cpp \
	  srcs/commands/Mode.cpp \
	  srcs/commands/ModeFlags.cpp

SRC_BONUS = srcs_bonus/main_bonus.cpp \
	  		srcs_bonus/Client_bonus.cpp \
	  		srcs_bonus/Server_bonus.cpp \
	  		srcs_bonus/ServerUtils_bonus.cpp \
	  		srcs_bonus/Channel_bonus.cpp \
	  		srcs_bonus/Command_bonus.cpp \
	  		srcs_bonus/commands_bonus/Join_bonus.cpp \
	  		srcs_bonus/commands_bonus/PrivMsg_bonus.cpp \
	  		srcs_bonus/commands_bonus/QuitCmd_bonus.cpp \
	  		srcs_bonus/commands_bonus/Kick_bonus.cpp \
	  		srcs_bonus/commands_bonus/Part_bonus.cpp \
	  		srcs_bonus/commands_bonus/Who_bonus.cpp \
	  		srcs_bonus/commands_bonus/Invite_bonus.cpp \
	  		srcs_bonus/commands_bonus/Topic_bonus.cpp \
	  		srcs_bonus/commands_bonus/Mode_bonus.cpp \
	  		srcs_bonus/commands_bonus/ModeFlags_bonus.cpp

OBJ = $(SRC:.cpp=.o)
OBJ_BONUS = $(SRC_BONUS:.cpp=.o)

CXX = c++

RM = rm -f

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -I Includes -I Includes_bonus

NAME = ircserv
BONUS_NAME = ircserv_bonus

all: $(NAME)

bonus: ${BONUS_NAME}

$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

$(BONUS_NAME): $(OBJ_BONUS)
	$(CXX) $(CPPFLAGS) $(OBJ_BONUS) -o $(BONUS_NAME)
	
	   
clean:
	$(RM) $(OBJ) $(OBJ_BONUS)

fclean: clean
	$(RM) $(NAME) $(BONUS_NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
