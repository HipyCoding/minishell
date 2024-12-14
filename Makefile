# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stalash <stalash@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/21 15:43:25 by christian         #+#    #+#              #
#    Updated: 2024/12/14 01:33:49 by stalash          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:= minishell
CFLAGS		:= -Wall -Wextra -Werror -g3 -fsanitize=address
CC			:= cc

#	thank you jorge <3

LIBFT_PATH = ./includes/libft
LIBFT = $(LIBFT_PATH)/libft.a

SRCS	:= main.c \
	parsing/tokenize.c \
	parsing/parse_utils_boolean.c \
	parsing/parse_utils.c \
	parsing/parse_quotations.c \
	parsing/parse_redirection.c \
	parsing/parse.c \
	parsing/cleanup.c \
	parsing/init_lexer.c \
	parsing/lexer.c \
	parsing/syntax_check.c \
	parsing/expansion.c \
	execution/execute.c \
	execution/system_commands.c \
	execution/buildins/redirections.c\
	execution/buildins/echo.c \
	execution/buildins/cd.c \
	execution/buildins/exit.c \
	execution/buildins/pwd.c \
	execution/buildins/env.c \
	execution/buildins/signal.c \
	execution/buildins/pipe.c \
	execution/buildins/export.c \
	execution/buildins/unset.c \
	execution/buildins/part_of_env.c \
	execution/buildins/part_of_export.c

OBJS	:= $(SRCS:%.c=bin/%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "\n$(NAME) build"

$(LIBFT):
	@make -sC $(LIBFT_PATH)
	@echo "libft build"

bin/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ && printf "\nCompiling: $(notdir $<)"

clean:
	@echo "cleaning..."
	@rm -rf $(OBJS)
	@make -sC $(LIBFT_PATH) clean

fclean: clean
	@echo "full clean..."
	@rm -rf $(NAME)
	@make -sC $(LIBFT_PATH) fclean

re: fclean all bonus

.PHONY: all clean fclean re bonus
