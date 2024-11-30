# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stalash <stalash@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/21 15:43:25 by christian         #+#    #+#              #
#    Updated: 2024/11/30 19:02:46 by stalash          ###   ########.fr        #
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
	parsing/syntax_check.c \
	execution/execute.c \
	execution/buildins/redirections.c\
	execution/buildins/echo.c \
	execution/buildins/cd.c \
	execution/buildins/exit.c \
	execution/buildins/pwd.c \
	execution/buildins/env.c \
	execution/buildins/signal.c

OBJS	:= $(SRCS:%.c=bin/%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
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
