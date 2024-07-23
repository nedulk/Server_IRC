# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kprigent <kprigent@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/17 12:10:05 by kprigent          #+#    #+#              #
#    Updated: 2024/07/23 13:57:30 by kprigent         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC = main.cpp \
	  bot.cpp

OBJ = $(SRC:.cpp=.o)

CXX = c++

RM = rm -f

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = bot

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)

.PHONY: all clean fclean re
