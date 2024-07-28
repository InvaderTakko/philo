# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sruff <sruff@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/29 15:18:08 by sruff             #+#    #+#              #
#    Updated: 2024/07/28 17:51:18 by sruff            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    := philo
CFLAGS  := -Wextra -Wall -Werror -g

# HEADERS := -I./include -I$(LIBMLX)/include -I . -I $(LIBFT)
SRCS    := philo.c
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