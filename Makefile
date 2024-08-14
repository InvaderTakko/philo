# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sruff <sruff@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/29 15:18:08 by sruff             #+#    #+#              #
#    Updated: 2024/08/14 21:32:07 by sruff            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    := philo
CFLAGS  := -Wextra -Wall -Werror -g

# cc philo_test.c utils.c init_cleanup.c time_and_simstop.c create_join_threads.c philo_eat.c -o philo
SRCS    := philo_test.c utils.c init_cleanup.c time_and_simstop.c create_join_threads.c philo_eat.c
OBJS    := ${SRCS:.c=.o}

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"
	
$(NAME): $(OBJS)
	@$(CC) $(OBJS) -o $(NAME)
	
clean:
	@rm -rf $(OBJS)
	
fclean: clean
	@rm -rf $(NAME)
	
re: fclean all

.PHONY: all clean fclean re 