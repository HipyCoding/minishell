/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 23:12:12 by candrese          #+#    #+#             */
/*   Updated: 2024/11/13 17:21:40 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

cmd_status execute_cmd_node(t_ast_node *node)
{
	if (!ft_strncmp(node->data, "echo", 5))
		return ft_echo(node);
	printf("Command not found: %s\n", node->data);
	return CMD_ERROR;
}

cmd_status execute_pipe_node(t_ast_node *node)
{
	cmd_status status;

	printf(">pipe<  - not implemented \n");
	status = execute_ast(node->left);
	if (status == CMD_SUCCESS)
		status = execute_ast(node->right);
	return status;
}

cmd_status execute_redir_node(t_ast_node *node)
{
	printf(">redirection< - not implemented \n");
	return execute_ast(node->left);
}

cmd_status execute_ast(t_ast_node *node)
{
	if (!node)
		return CMD_ERROR;
	if (node->type == NODE_PIPE)
		return execute_pipe_node(node);
	else if (node->type == NODE_REDIR) 
		return execute_redir_node(node);
	else if (node->type == NODE_CMD)
		return execute_cmd_node(node);
	printf("Unknown node type\n");
	return CMD_ERROR;
}
