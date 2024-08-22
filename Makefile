# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sruff <sruff@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/29 15:18:08 by sruff             #+#    #+#              #
#    Updated: 2024/08/22 13:35:23 by sruff            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    := philo
CC      := cc
CFLAGS  := -Wextra -Wall -Werror -O3

# cc philo_test.c utils.c init_cleanup.c time_and_simstop.c create_join_threads.c philo_eat.c -o philo
SRCS    := philo.c utils.c init_cleanup.c time_and_simstop.c create_join_threads.c philo_eat.c
OBJS    := ${SRCS:.c=.o}

all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"
	
$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	@echo "Compilation complete. Executable '$(NAME)' created."
	
clean:
	@rm -f $(OBJS)
	@echo "Object files removed."
	
fclean: clean
	@rm -f $(NAME)
	@echo "Executable '$(NAME)' removed."
	
re: fclean all

.PHONY: all clean fclean re 