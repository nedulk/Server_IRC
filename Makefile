# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/17 12:10:05 by kprigent          #+#    #+#              #
#    Updated: 2024/07/14 16:44:35 by kprigent         ###   ########.fr        #
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
	  srcs/commands/QuitCmd.cpp

OBJ = $(SRC:.cpp=.o)

CXX = c++

RM = rm -f

CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -I Includes

NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re