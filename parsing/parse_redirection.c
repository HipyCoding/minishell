/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 01:31:21 by stalash           #+#    #+#             */
/*   Updated: 2024/12/14 02:35:33 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast_node	*parse_redirection(t_token **tokens)
{
	t_ast_node	*redir_node;
	t_token		*current;

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
