/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 23:12:12 by candrese          #+#    #+#             */
/*   Updated: 2024/11/14 01:39:34 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// this funcion is to chanfe the commad arg AST into char**

char **get_command_arguments(t_ast_node *cmd_node)
{
	int argc;
	t_ast_node *arg_node;
	char **args;
	int i;

	argc = 0;
	i = 0;
	arg_node = cmd_node->args;
	while (arg_node)
	{
		argc++;
		arg_node = arg_node->next;
	}
	args = (char **)malloc((argc + 2) * sizeof(char *));  // +2 for cmd and NULL
	if (!args)
	{
		perror("malloc");
		return NULL;
	}
	// Add command as first argument
	args[i++] = ft_strdup(cmd_node->data);
	arg_node = cmd_node->args;
	while (arg_node)
	{
		args[i++] = ft_strdup(arg_node->data);
		arg_node = arg_node->next;
	}
	args[i] = NULL;
	return (args);
}

cmd_status execute_external(t_ast_node *cmd_node)
{
	pid_t pid;
	int status;
	char **args;
	cmd_status ret;

	if ((pid = fork()) == -1)
	{
		perror("fork");
		return CMD_ERROR;
	}
	if (pid == 0)
	{
		args = get_command_arguments(cmd_node);
		if (!args)
			exit(CMD_ERROR);
		execvp(args[0], args);
		int i = 0;
		while (args[i])
			free(args[i++]);
		free(args);
		perror("execvp");
		exit(CMD_ERROR);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
		else
			ret = CMD_ERROR;
		return ret;
	}
}


cmd_status	execute_builtin(t_ast_node *cmd_node)
{
	if (ft_strncmp(cmd_node->data, "echo", 5) == 0)
	{
		printf("execute echo buildin here\n");
		return (ft_echo(cmd_node));
	}
		//return (ft_echo(cmd_node));
	else if (ft_strncmp(cmd_node->data, "cd", 3) == 0)
	{
		printf("execute cd buildin here\n");
		return CMD_SUCCESS;
	}
		//return (ft_cd(cmd_node));
	else if (ft_strncmp(cmd_node->data, "pwd", 4) == 0)
	{
		printf("execute pwd buildin here\n");
		return CMD_SUCCESS;
	}
		//return (ft_pwd());
	else if (ft_strncmp(cmd_node->data, "export", 7) == 0)
	{
		printf("execute export buildin here\n");
		return CMD_SUCCESS;
	}
		//return (ft_export(cmd_node));
	else if (ft_strncmp(cmd_node->data, "unset", 6) == 0)
	{
		printf("execute unset buildin here\n");
		return CMD_SUCCESS;
	}
		//return (ft_unset(cmd_node));
	else if (ft_strncmp(cmd_node->data, "env", 4) == 0)
	{
		printf("execute env buildin here\n");
		return CMD_SUCCESS;
	}
		//return (ft_env(cmd_node));
	else if (ft_strncmp(cmd_node->data, "exit", 5) == 0)
	{
		printf("execute exit buildin here\n");
		return CMD_SUCCESS;
	}
		//return (ft_exit());
	printf("CMD ERROR in execute_builtin\n");
	return (CMD_ERROR);
}

bool	is_builtin(char *cmd)
{
	int	i;
	const char	*builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit", NULL};

	i = 0;
	while (builtins[i])
	{
		if (ft_strncmp(cmd, builtins[i], ft_strlen(builtins[i] + 1)) == 0)
			return (true);
		i++;
	}
	return (false);
}

cmd_status execute_cmd_node(t_ast_node *node)
{
	if (is_builtin(node->data))
		return execute_builtin(node);
	else
		return execute_external(node);
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
