/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 23:12:12 by candrese          #+#    #+#             */
/*   Updated: 2024/12/13 19:35:49 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

cmd_status	execute_builtin(t_ast_node *cmd_node, t_shell *shell)
{
	if (ft_strncmp(cmd_node->data, "echo", 5) == 0)
		return (ft_echo(cmd_node));
	else if (ft_strncmp(cmd_node->data, "cd", 3) == 0)
		return (ft_cd(cmd_node));
	else if (ft_strncmp(cmd_node->data, "pwd", 4) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd_node->data, "export", 7) == 0)
		return (ft_export(cmd_node, shell));
	else if (ft_strncmp(cmd_node->data, "unset", 6) == 0)
		return (ft_unset(cmd_node, &(*shell).env_list));
	else if (ft_strncmp(cmd_node->data, "env", 4) == 0)
		return (ft_env(cmd_node, shell->env_list));
	else if (ft_strncmp(cmd_node->data, "exit", 5) == 0)
		return (ft_exit(cmd_node, shell));
	return (printf("CMD ERROR in execute_builtin\n"), CMD_ERROR);
}

bool	is_builtin(char *cmd)
{
	int			i;
	const char	*builtins[] = {"echo", "cd", "pwd", "export", \
							"unset", "env", "exit", NULL};

	i = 0;
	while (builtins[i])
	{
		if (ft_strncmp(cmd, builtins[i], ft_strlen(builtins[i]) + 1) == 0)
			return (true);
		i++;
	}
	return (false);
}

cmd_status	execute_cmd_node(t_ast_node *node, t_shell *shell)
{
	if (is_builtin(node->data))
		return (execute_builtin(node, shell));
	else
		return (execute_external(node, shell));
	return (printf("Command not found: %s\n", node->data), CMD_ERROR);
}

cmd_status	execute_pipe_node(t_ast_node *node, t_shell *shell)
{
	cmd_status	status;

	if (node->type == NODE_PIPE)
		return (execute_pipeline(node, shell));
	status = execute_ast(node->left, shell);
	if (status == CMD_SUCCESS)
		status = execute_ast(node->right, shell);
	return (status);
}

cmd_status	execute_ast(t_ast_node *node, t_shell *shell)
{
	cmd_status	status;

	status = CMD_ERROR;
	if (!node)
		return (CMD_ERROR);
	if (node->type == NODE_PIPE)
		status = execute_pipe_node(node, shell);
	else if (node->type == NODE_REDIR)
		status = handle_redirection(node, shell);
	else if (node->type == NODE_CMD)
		status = execute_cmd_node(node, shell);
	shell->exit_status = status;
	return (status);
}
