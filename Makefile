# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/15 17:17:26 by macerver          #+#    #+#              #
#    Updated: 2026/06/21 12:02:52 by macerver         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := codexion
CC := cc
CFLAGS := -Wall -Werror -Wextra -O0 -g -I. 
SRC = actions.c codexion.c heap.c monitor.c parse_args.c routines.c utils.c

OBJ = $(SRC:.c=.o)

RM := rm -f

all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lpthread

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) 

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re