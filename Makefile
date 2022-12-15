# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pnuti <pnuti@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/21 09:52:49 by pnuti             #+#    #+#              #
#    Updated: 2022/12/14 17:12:01 by pnuti            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		ft_containers

CC =		c++
SRC =		main.cpp
OBJ =		$(SRC:%.cpp=%.o)
OBJ_DIR =	obj
CFLAGS =	-Wall -Werror -Wextra -std=c++98 $(IFLAGS)
IFLAGS =	-I ./containers -I ./iterators -I ./utils

all : $(NAME)

$(NAME) : $(OBJ_DIR)/$(OBJ)
	$(CC) $(CFLAGS) $(OBJ_DIR)/$(OBJ) -o $(NAME)

$(OBJ_DIR)/$(OBJ) : $(SRC)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -fr $(OBJ_DIR)

fclean : clean
	rm -f $(NAME) $(NAME_STD)

re : fclean all

.PHONY: clean fclean re all