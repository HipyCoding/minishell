# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: candrese <candrese@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/21 15:43:25 by christian         #+#    #+#              #
#    Updated: 2024/10/25 18:39:50 by candrese         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
CFLAGS		:= -Wall -Wextra -Werror #-g3 #-fsanitize=address
CC			:= cc

#		 thank you jorge <3

LIBFT_PATH = ./includes/libft
LIBFT = $(LIBFT_PATH)/libft.a

SRCS	:= main.c \
	parsing/parse_utils.c\
	parsing/tokenize.c

OBJS	:= $(SRCS:%.c=bin/%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) -lreadline $(LIBFT) $(OBJS) -o $(NAME)
	@echo "building $(NAME)"

$(LIBFT):
	make -C $(LIBFT_PATH)

bin/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ && printf "\nCompiling: $(notdir $<)"

clean:
	@echo "cleaning..."
	@rm -rf $(OBJS)

fclean: clean
	@echo "full clean..."
	@rm -rf $(NAME)

re: fclean all bonus

.PHONY: all clean fclean re bonus
