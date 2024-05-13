# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/27 01:49:33 by msaadidi          #+#    #+#              #
#    Updated: 2024/05/11 10:12:55 by msaadidi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRCS = philo.c
OBJS = $(SRCS:.c=.o)
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
CC = cc

all : $(NAME)



$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $(SRCS) -o $@
	@make clean
	
clean :
	@rm -f $(OBJS)

fclean : clean
	@rm -f $(NAME)

re : fclean all