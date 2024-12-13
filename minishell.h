/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 02:50:52 by candrese          #+#    #+#             */
/*   Updated: 2024/12/13 17:27:08 by stalash          ###   ########.fr       */
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

# define SINGLE_QUOTE_MARK "!+S_QTmk1+!"

# define RESET_COLOR    "\033[0m"
# define BRIGHT_RED     "\033[0;91m"
# define BRIGHT_GREEN   "\033[0;92m"

// Status codes for command execution
typedef enum
{
	CMD_SUCCESS = 0,
	CMD_ERROR = 1,
}		cmd_status;

//  environment struct
typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
}	t_env;

// shell struct
typedef struct s_shell
{
	t_env *env_list;
	int exit_status;
} t_shell;

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

// Struct for error handling within the syntax checks
typedef enum
{
	SYNTAX_OK = 0,
	ERR_EMPTY_PIPE,
	ERR_INVALID_REDIR,
	ERR_CMD_NOT_FOUND,
	ERR_CONSECUTIVE_PIPE,
	ERR_MISSING_REDIR_FILE,
	ERR_UNCLOSED_QUOTE
} 		syntax_error_t;

// lexing
t_ast_node		*create_ast_node(ast_type type, char *data);
t_token			*create_token(ast_type type, char *data);
char			*handle_special_char(const char *input, int *i);
int				get_word_length(const char *input, int start);
char			*handle_word(const char *input, int *i);
char			*extract_token_data(const char *input, int *i);
ast_type		get_basic_token_type(char first_char);
ast_type		get_token_type(const char *data, const t_token *prev_token);
void			add_token(t_token **head, t_token *new_token);
t_token *lexer(const char *input, t_shell *shell);
void			cleanup_tokens(t_token *head);
void			cleanup_env_list(t_env *env_list);
t_ast_node		*parse_command(t_token **tokens);
t_ast_node		*parse_redirection(t_token **tokens);
t_ast_node		*parse_command_with_redirections(t_token **tokens);
t_ast_node		*parse_pipeline(t_token **tokens);
t_ast_node		*parse(t_token *tokens, t_ast_node *ast,t_shell *shell, cmd_status *status);
void			print_ast(t_ast_node *node, int depth);
void free_2d_string(char **array);
void			free_ast(t_ast_node *node);
void			free_tokens(t_token *head);
char			*extract_env_var_name(const char *input, int *i);
char *handle_quoted_string(const char *input, int *i, t_shell *shell);
char *get_env_value(t_env *env_list, const char *key);
t_token	*init_new_token(const char *input, int *i, t_token *prev_token, t_shell *shell);
void print_tokens(t_token *head);

// lexing utils
bool			is_special_char(char c);
bool			is_whitespace(char c);
bool			is_quote(char c);
bool			skip_whitespace(const char *input, int *i);

// syntax checks
bool			is_valid_command(const char *cmd);
syntax_error_t	check_command_syntax(t_ast_node *cmd_node);
syntax_error_t	check_redirection_syntax(t_ast_node *redir_node);
syntax_error_t	check_syntax(t_ast_node *node);
void			display_syntax_error(syntax_error_t error);

// env
t_env			*init_env(char **envp);
cmd_status		ft_env(t_ast_node *cmd_node, t_env *env_list);
void			expand_env_vars_in_node(t_ast_node *node, t_shell *shell);
t_env			*create_env_node(char *key, char *value);
bool			split_env_str(char *env_str, char **key, char **value);
void			add_env_node(t_env **env_list, t_env *new_node);

// builtin functions
cmd_status		ft_echo(t_ast_node *cmd_node);
cmd_status		ft_cd(t_ast_node *cmd_node);
cmd_status		ft_exit(t_ast_node *cmd_node, t_shell *shell);
cmd_status		ft_pwd();
void			setup_signal_handlers();
cmd_status		handle_redirection(t_ast_node *redir_node, t_shell *shell);
cmd_status		execute_pipeline(t_ast_node *node, t_shell *shell);
cmd_status ft_export(t_ast_node *cmd_node, t_shell *shell);
cmd_status		ft_unset(t_ast_node *cmd_node, t_env **env_list);

cmd_status		execute_ast(t_ast_node *node, t_shell *shell);
cmd_status		execute_external(t_ast_node *cmd_node, t_shell *shell);

#endif
