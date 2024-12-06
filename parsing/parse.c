/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: christian <christian@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 02:46:55 by candrese          #+#    #+#             */
/*   Updated: 2024/12/06 16:01:31 by christian        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast_node *parse_command(t_token **tokens)
{
	t_ast_node *cmd_node;
	t_ast_node *arg_node;
	t_ast_node *last_arg;
	t_token *current;

	if (!tokens || !*tokens || (*tokens)->type != NODE_CMD)
		return NULL;
	current = *tokens;
	cmd_node = create_ast_node(NODE_CMD, ft_strdup(current->data));
	if (!cmd_node)
		return NULL;
	current = current->next;
	last_arg = NULL;
	// handle arguments until we hit a pipe or redirection or end
	while (current && current->type == NODE_ARG)
	{
		arg_node = create_ast_node(NODE_ARG, ft_strdup(current->data));
		if (!arg_node)
		{
			free_ast(cmd_node);
			return NULL;
		}
		// Add argument to end of list
		if (!cmd_node->args)
			cmd_node->args = arg_node;
		else
		{
			if (!last_arg)
			{
				// we find the last argument
				last_arg = cmd_node->args;
				while (last_arg->next)
					last_arg = last_arg->next;
			}
			last_arg->next = arg_node;
			last_arg = arg_node;
		}
		current = current->next;
	}
	*tokens = current;
	return cmd_node;
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
	if (ft_strncmp(current->data, ">", 3) == 0)
		redir_node->redir_type = 1;
	else if (ft_strncmp(current->data, "<", 3) == 0)
		redir_node->redir_type = 2;
	else if (ft_strncmp(current->data, ">>", 3) == 0)
		redir_node->redir_type = 3;
	else if (ft_strncmp(current->data, "<<", 3) == 0)
		redir_node->redir_type = 4;
	current = current->next;
	redir_node->right = create_ast_node(NODE_WORD, ft_strdup(current->data));
	*tokens = current->next;
	return (redir_node);
}

// Parse a command with its redirections
t_ast_node *parse_command_with_redirections(t_token **tokens)
{
	t_ast_node *cmd_node;
	t_ast_node *redir_node;
	t_token *current;

	// First parse the command itself
	cmd_node = parse_command(tokens);
	if (!cmd_node)
		return NULL;
	current = *tokens;
	// Handle any redirections attached to this command
	while (current && current->type == NODE_REDIR)
	{
		redir_node = parse_redirection(&current);
		if (!redir_node)
		{
			free_ast(cmd_node);
			return NULL;
		}
		redir_node->left = cmd_node;
		cmd_node = redir_node;
		*tokens = current;
	}
	return cmd_node;
}

// Modified parse_pipeline to keep pipes at the top
t_ast_node *parse_pipeline(t_token **tokens)
{
	t_ast_node *pipe_node;
	t_ast_node *left_cmd;
	t_token *current;

	if (!tokens || !*tokens)
		return NULL;
	// Parse the left command (including its redirections)
	left_cmd = parse_command_with_redirections(tokens);
	if (!left_cmd)
		return NULL;
	current = *tokens;
	// If no pipe, return just the command with its redirections
	if (!current || current->type != NODE_PIPE)
		return left_cmd;
	// Create pipe node
	pipe_node = create_ast_node(NODE_PIPE, NULL);
	if (!pipe_node)
	{
		free_ast(left_cmd);
		return NULL;
	}
	*tokens = current->next;
	// Recursively parse the right side
	pipe_node->left = left_cmd;
	pipe_node->right = parse_pipeline(tokens);
	if (!pipe_node->right)
	{
		free_ast(pipe_node);
		return NULL;
	}
	return pipe_node;
}

// can add more parsing modules here
t_ast_node *parse(t_token *tokens, t_ast_node *ast,t_shell *shell, cmd_status *status)
{
	syntax_error_t	error;

	ast = parse_pipeline (&tokens);
	if (!ast)
		return NULL;
	error = check_syntax (ast);
	print_ast(ast, 0);
	if (error != SYNTAX_OK)
	{
		display_syntax_error (error);
		cleanup_tokens(tokens);
		free_ast(ast);
		*status = CMD_ERROR;
		return NULL;
	}
	expand_env_vars_in_node(ast, shell);
	return ast;
}
