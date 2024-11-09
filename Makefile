# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: candrese <candrese@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/21 15:43:25 by christian         #+#    #+#              #
#    Updated: 2024/11/09 23:24:07 by candrese         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
CFLAGS		:= -Wall -Wextra -Werror -g3 #-fsanitize=address
CC			:= cc

#		 thank you jorge <3

LIBFT_PATH = ./includes/libft
LIBFT = $(LIBFT_PATH)/libft.a

SRCS	:= main.c \
	parsing/tokenize.c \
	parsing/parse_utils_boolean.c \
	parsing/parse_utils.c \
	parsing/parse.c \
	parsing/cleanup.c \
	parsing/init_lexer.c \
	parsing/syntax_check.c  \
	execution/execute.c \
	execution/buildins/echo.c

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
