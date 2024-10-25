# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: christian <christian@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/21 15:43:25 by christian         #+#    #+#              #
#    Updated: 2024/10/25 07:19:49 by christian        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
CFLAGS		:= -Wall -Wextra -Werror -g3 #-fsanitize=address

SRCS	:= main.c parsing/parse_utils.c

OBJS	:= ${SRCS:.c=.o}

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ && printf "\nCompiling: $(notdir $<)"

clean:
	@echo "cleaning..."
	@rm -rf $(OBJS) $(BONUS_OBJ)

fclean: clean
	@echo "full clean..."
	@rm -rf $(NAME) $(BONUS_NAME)

re: fclean all bonus

.PHONY: all clean fclean re bonus
