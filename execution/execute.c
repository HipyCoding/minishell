/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 23:12:12 by candrese          #+#    #+#             */
/*   Updated: 2024/11/13 17:54:50 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}
// the executable cmd executable functions in side of this functoin should be made
cmd_status	execute_builtin(t_ast_node *cmd_node)
{
	if (ft_strcmp(cmd_node->data, "echo") == 0)
		return (ft_echo(cmd_node));
	else if (ft_strcmp(cmd_node->data, "cd") == 0)
		return (ft_cd(cmd_node));
	else if (ft_strcmp(cmd_node->data, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd_node->data, "export") == 0)
		return (ft_export(cmd_node));
	else if (ft_strcmp(cmd_node->data, "unset") == 0)
		return (ft_unset(cmd_node));
	else if (ft_strcmp(cmd_node->data, "env") == 0)
		return (ft_env(cmd_node));
	else if (ft_strcmp(cmd_node->data, "exit") == 0)
		return (ft_exit());
	return (CMD_ERROR);
}

bool	is_builtin(char *cmd)
{
	int	i;
	const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};

	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

cmd_status	execute_ast(t_ast_node *node)
{
	if (!node)
		return (CMD_ERROR);

	if (node->type)
	{
		if (NODE_CMD)
		{
			if (is_builtin(node->data))
				return execute_builtin(node);
				// implement execute_external function in order to execute externall cmd commands
			// else
				// return execute_external(node);
		}
	}
}

// cmd_status execute_cmd_node(t_ast_node *node)
// {
// 	if (!ft_strncmp(node->data, "echo", 5))
// 		return ft_echo(node);
// 	printf("Command not found: %s\n", node->data);
// 	return CMD_ERROR;
// }

// cmd_status execute_pipe_node(t_ast_node *node)
// {
// 	cmd_status status;

// 	printf(">pipe<  - not implemented \n");
// 	status = execute_ast(node->left);
// 	if (status == CMD_SUCCESS)
// 		status = execute_ast(node->right);
// 	return status;
// }

// cmd_status execute_redir_node(t_ast_node *node)
// {
// 	printf(">redirection< - not implemented \n");
// 	return execute_ast(node->left);
// }

// cmd_status execute_ast(t_ast_node *node)
// {
// 	if (!node)
// 		return CMD_ERROR;
// 	if (node->type == NODE_PIPE)
// 		return execute_pipe_node(node);
// 	else if (node->type == NODE_REDIR)
// 		return execute_redir_node(node);
// 	else if (node->type == NODE_CMD)
// 		return execute_cmd_node(node);
// 	printf("Unknown node type\n");
// 	return CMD_ERROR;
// }
