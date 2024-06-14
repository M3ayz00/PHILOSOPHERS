# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/27 01:49:33 by msaadidi          #+#    #+#              #
#    Updated: 2024/06/14 16:12:29 by msaadidi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread
SRCS =  philo.c atoi.c parsing.c initialization.c philo_actions.c philo_utils.c philo_utils2.c
OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	@$(CC) $(CFLAGS) $(SRCS) -o $@
	@make clean
	
clean :
	@rm -f $(OBJS)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : make re clean fclean

.SECOND : $(OBJS)