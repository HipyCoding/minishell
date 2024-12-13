/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: christian <christian@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 02:46:55 by candrese          #+#    #+#             */
/*   Updated: 2024/12/13 13:46:35 by christian        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_ast_node	*handle_new_argument(t_ast_node *cmd_node,
					t_ast_node **last_arg, char *arg_data)
{
	t_ast_node	*arg_node;

	arg_node = create_ast_node(NODE_ARG, ft_strdup(arg_data));
	if (!arg_node)
	{
		free_ast(cmd_node);
		return (NULL);
	}
	if (!cmd_node->args)
		cmd_node->args = arg_node;
	else
	{
		if (!*last_arg)
		{
			*last_arg = cmd_node->args;
			while ((*last_arg)->next)
				*last_arg = (*last_arg)->next;
		}
		(*last_arg)->next = arg_node;
		*last_arg = arg_node;
	}
	return (arg_node);
}

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*cmd_node;
	t_ast_node	*last_arg;
	t_token		*current;

	if (!tokens || !*tokens || (*tokens)->type != NODE_CMD)
		return (NULL);
	current = *tokens;
	cmd_node = create_ast_node(NODE_CMD, ft_strdup(current->data));
	if (!cmd_node)
		return (NULL);
	current = current->next;
	last_arg = NULL;
	while (current && current->type == NODE_ARG)
	{
		if (!handle_new_argument(cmd_node, &last_arg, current->data))
			return (NULL);
		current = current->next;
	}
	*tokens = current;
	return (cmd_node);
}

t_ast_node *parse_redirection(t_token **tokens)
{
	t_ast_node *redir_node;
	t_token *current;

	current = *tokens;
	if (!current || !current->next)
		return (NULL);
	redir_node = create_ast_node(NODE_REDIR, NULL);
	if (!redir_node)
		return (NULL);
	if (ft_strncmp(current->data, "<<", 3) == 0)
		redir_node->redir_type = 4;
	else if (ft_strncmp(current->data, "<", 3) == 0)
		redir_node->redir_type = 2;
	else if (ft_strncmp(current->data, ">>", 3) == 0)
		redir_node->redir_type = 3;
	else if (ft_strncmp(current->data, ">", 3) == 0)
		redir_node->redir_type = 1;
	current = current->next;
	redir_node->right = create_ast_node(NODE_WORD, ft_strdup(current->data));
	*tokens = current->next;
	return (redir_node);
}

// Parse a command with its redirections
static t_ast_node	*handle_initial_redirection(t_token **tokens)
{
	t_ast_node	*redir_node;
	t_ast_node	*cmd_node;
	t_token		*current;

	current = *tokens;
	redir_node = parse_redirection(&current);
	if (!redir_node)
		return (NULL);
	*tokens = current;
	if (*tokens && (*tokens)->type != NODE_REDIR)
	{
		cmd_node = parse_command(tokens);
		if (cmd_node)
			redir_node->left = cmd_node;
	}
	return (redir_node);
}

static t_ast_node	*handle_command_redirections(t_token **tokens)
{
	t_ast_node	*cmd_node;
	t_ast_node	*redir_node;
	t_token		*current;

	cmd_node = parse_command(tokens);
	if (!cmd_node)
		return (NULL);
	current = *tokens;
	while (current && current->type == NODE_REDIR)
	{
		redir_node = parse_redirection(&current);
		if (!redir_node)
		{
			free_ast(cmd_node);
			return (NULL);
		}
		redir_node->left = cmd_node;
		cmd_node = redir_node;
		*tokens = current;
	}
	return (cmd_node);
}

t_ast_node	*parse_command_with_redirections(t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	if (current && current->type == NODE_REDIR)
		return (handle_initial_redirection(tokens));
	return (handle_command_redirections(tokens));
}

static t_ast_node	*create_pipe_node(t_token **tokens, t_ast_node *left_cmd)
{
	t_ast_node	*pipe_node;

	pipe_node = create_ast_node(NODE_PIPE, NULL);
	if (!pipe_node)
	{
		free_ast(left_cmd);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	pipe_node->left = left_cmd;
	pipe_node->right = parse_pipeline(tokens);
	if (!pipe_node->right)
	{
		free_ast(pipe_node);
		return (NULL);
	}
	cleanup_tokens(*tokens);
	return (pipe_node);
}

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left_cmd;
	t_token		*current;

	if (!tokens || !*tokens)
		return (NULL);
	left_cmd = parse_command_with_redirections(tokens);
	if (!left_cmd)
		return (NULL);
	current = *tokens;
	if (!current || current->type != NODE_PIPE)
		return (left_cmd);
	return (create_pipe_node(tokens, left_cmd));
}

// can add more parsing modules here
t_ast_node *parse(t_token *tokens, t_ast_node *ast,t_shell *shell, cmd_status *status)
{
	syntax_error_t	error;

	ast = parse_pipeline (&tokens);
	if (!ast)
	{
		cleanup_tokens(tokens);
		*status = CMD_ERROR;
		return NULL;
	}
	error = check_syntax (ast);
	// print_ast(ast, 0);
	if (error != SYNTAX_OK)
	{
		display_syntax_error (error);
		cleanup_tokens(tokens);
		free_ast(ast);
		*status = CMD_ERROR;
		return NULL;
	}
	expand_env_vars_in_node(ast, shell);
	cleanup_tokens(tokens);
	return ast;
}
