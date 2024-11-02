/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 02:50:52 by candrese          #+#    #+#             */
/*   Updated: 2024/11/02 08:49:55 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


# include <stdio.h>
# include "./includes/libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum
{
	NODE_PIPE,		// Pipeline node (connects commands)
	NODE_CMD,		// Command node (includes command name and args)
	NODE_REDIR,		// Redirection node
	NODE_ARG,		// Argument node
	NODE_WORD,		// Word node (for command names)
	NODE_ENV,		// Environment variable
	NODE_QUOTE,		// Quoted string
	NODE_ERROR,
} ast_type;

// AST node structure
typedef struct s_ast_node
{
	ast_type type;
	char *data;
	struct s_ast_node *left;	// Left child
	struct s_ast_node *right;	// Right child
	struct s_ast_node *args;	// For command arguments (linked list)
	struct s_ast_node *next;	// For argument lists
	int redir_type;				// For redirection nodes (>, <, >>, <<)
} t_ast_node;

// Token structure for lexical analysis
typedef struct s_token
{
	ast_type type;
	char *data;
	struct s_token *next;
} t_token;

// lexing
t_ast_node		*create_ast_node(ast_type type, char *data);
t_token			*create_token(ast_type type, char *data);
char			*handle_special_char(const char *input, int *i);
int				get_word_length(const char *input, int start);
char			*handle_word(const char *input, int *i);
char			*extract_token_data(const char *input, int *i);
ast_type		get_basic_token_type(char first_char);
ast_type		get_token_type(const char *data, const t_token *prev_token);
t_token			*init_new_token(const char *input, int *i, t_token *prev_token);
void			add_token(t_token **head, t_token *new_token);
t_token			*lexer(const char *input);
void			cleanup_tokens(t_token *head);
t_ast_node		*parse_command(t_token **tokens);
t_ast_node		*parse_redirection(t_token **tokens);
t_ast_node		*parse_command_with_redirections(t_token **tokens);
t_ast_node		*parse_pipeline(t_token **tokens);
t_ast_node		*parse(t_token *tokens);
void			print_ast(t_ast_node *node, int depth);
void			free_ast(t_ast_node *node);
void			free_tokens(t_token *head);
char 			*extract_env_var_name(const char *input, int *i);

bool is_special_char(char c);
bool is_whitespace(char c);
bool is_quote(char c);
bool skip_whitespace(const char *input, int *i);

#endif
