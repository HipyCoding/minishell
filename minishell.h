/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 02:50:52 by candrese          #+#    #+#             */
/*   Updated: 2024/12/14 03:58:06 by stalash          ###   ########.fr       */
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
typedef enum e_cmd_status
{
	CMD_SUCCESS = 0,
	CMD_ERROR = 1,
}		t_cmd_status;

//  environment struct
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}		t_env;

// shell struct
typedef struct s_shell
{
	t_env	*env_list;
	int		exit_status;
}	t_shell;

typedef enum e_ast_type
{
	NODE_PIPE,
	NODE_CMD,
	NODE_REDIR,
	NODE_ARG,
	NODE_WORD,
	NODE_ENV,
	NODE_QUOTE,
	NODE_ERROR,
}		t_ast_type;

// AST node structure
typedef struct s_ast_node
{
	t_ast_type			type;
	char				*data;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*args;
	struct s_ast_node	*next;
	int					redir_type;
}		t_ast_node;

// Token structure for lexical analysis
typedef struct s_token
{
	t_ast_type		type;
	char			*data;
	struct s_token	*next;
}		t_token;

// Struct for error handling within the syntax checks
typedef enum e_syntax_error {
	SYNTAX_OK = 0,
	ERR_EMPTY_PIPE,
	ERR_INVALID_REDIR,
	ERR_CMD_NOT_FOUND,
	ERR_CONSECUTIVE_PIPE,
	ERR_MISSING_REDIR_FILE,
	ERR_UNCLOSED_QUOTE
}		t_syntax_error;

// cleanup
void			cleanup_tokens(t_token *head);
void			cleanup_env_list(t_env *env_list);
void			free_2d_string(char **array);
void			free_ast(t_ast_node *node);

// parse abstract syntax tree
t_ast_node		*parse(t_token *tokens, t_ast_node *ast, t_shell *shell, \
				t_cmd_status *t_status);
t_ast_node		*parse_pipeline(t_token **tokens);
t_ast_node		*parse_command(t_token **tokens);
t_ast_node		*parse_redirection(t_token **tokens);
t_ast_node		*parse_command_with_redirections(t_token **tokens);
t_ast_node		*create_ast_node(t_ast_type type, char *data);

// lexing
char			*handle_quoted_string(const char *input, int *i, \
									t_shell *shell);
t_ast_type		get_basic_token_type(char first_char);
t_ast_type		get_token_type(const char *data, const t_token *prev_token);
void			add_token(t_token **head, t_token *new_token);
char			*handle_word(const char *input, int *i);
char			*extract_token_data(const char *input, int *i);
t_token			*create_token(t_ast_type type, char *data);
t_token			*init_new_token(const char *input, int *i, \
								t_token *prev_token, t_shell *shell);
char			*extract_token_data(const char *input, int *i);
char			*extract_env_var_name(const char *input, int *i);
t_token			*lexer(const char *input, t_shell *shell);
char			*handle_special_char(const char *input, int *i);
char			*get_env_value(t_env *env_list, const char *key);

// debugging
void			print_tokens(t_token *head);
void			print_ast(t_ast_node *node, int depth);

// lexing utils
bool			has_unclosed_quotes(const char *input);
bool			is_special_char(char c);
bool			is_whitespace(char c);
bool			is_quote(char c);
bool			skip_whitespace(const char *input, int *i);
int				get_word_length(const char *s, int st, int l);
char			*handle_word(const char *input, int *i);

// syntax checks
bool			is_valid_command(const char *cmd);
t_syntax_error	check_command_syntax(t_ast_node *cmd_node);
t_syntax_error	check_redirection_syntax(t_ast_node *redir_node);
t_syntax_error	check_syntax(t_ast_node *node, t_syntax_error error);
void			display_syntax_error(t_syntax_error error);

// env
t_env			*init_env(char **envp);
t_cmd_status		ft_env(t_ast_node *cmd_node, t_env *env_list);
void			expand_env_vars_in_node(t_ast_node *node, t_shell *shell);
t_env			*create_env_node(char *key, char *value);
bool			split_env_str(char *env_str, char **key, char **value);
void			add_env_node(t_env **env_list, t_env *new_node);

// builtin functions
t_cmd_status		ft_echo(t_ast_node *cmd_node);
t_cmd_status		ft_cd(t_ast_node *cmd_node);
t_cmd_status		ft_exit(t_ast_node *cmd_node, t_shell *shell);
t_cmd_status		ft_pwd(void);
void			setup_signal_handlers(void);
t_cmd_status		handle_redirection(t_ast_node *redir_node, t_shell *shell);
t_cmd_status		execute_pipeline(t_ast_node *node, t_shell *shell);
t_cmd_status		ft_export(t_ast_node *cmd_node, t_shell *shell);
t_cmd_status		ft_unset(t_ast_node *cmd_node, t_env **env_list);

// Part of export function
t_env			*find_env_var(t_env *env_list, const char *key);
int				count_env_nodes(t_env *env_list);
t_env			**fill_env_array(t_env *env_list, int count);
void			sort_env_array(t_env **arr, int count);
void			print_env_array(t_env **arr, int count);

// execute
t_cmd_status		execute_ast(t_ast_node *node, t_shell *shell);
t_cmd_status		execute_external(t_ast_node *cmd_node, t_shell *shell);

#endif
