# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: christian <christian@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/21 15:43:25 by christian         #+#    #+#              #
#    Updated: 2024/09/21 15:49:53 by christian        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror #-g3
INCLUDE = -L $(LIBFT_PATH) -lft

LIBFT_PATH = ./lib
LIBFT = $(LIBFT_PATH)/libft.a

SRCS = \
	src/main.C
	

OBJS = $(SRCS:%.c=$(BINDIR)/%.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(LIBFT) -o $(NAME) $(OBJS) $(INCLUDE) $(RL_FLAGS)
	echo $(GREEN)"Building $(NAME)"$(DEFAULT);

$(LIBFT):
	make -C $(LIBFT_PATH)

	clean:
	make clean -C $(LIBFT_PATH)
	rm -rf $(BINDIR)
	echo $(RED)"Removing $(NAME) object files"$(DEFAULT);

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_PATH)
	echo $(RED)"Removing $(NAME)"$(DEFAULT);

re: fclean all
	echo $(GREEN)"Rebuilding everything"$(DEFAULT);

.PHONY: all clean fclean re