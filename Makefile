# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: macerver <macerver@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/15 17:17:26 by macerver          #+#    #+#              #
#    Updated: 2026/06/15 17:19:35 by macerver         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := codexion
CC := cc
CFLAGS := -Wall -Werror -Wextra -O0 -g -I. 
SRC = actions.c codexion.c parse_args.c queue.c routines.c utils.c

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

bonus: $(OBJ)
	$(AR) $(NAME) $^

.PHONY: all clean fclean re