/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:17:29 by christian         #+#    #+#             */
/*   Updated: 2024/12/11 12:17:28 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char **create_env_array(t_env *env_list)
{
	int count = 0;
	t_env *current;
	char *tmp;
	char **env_array;

	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return NULL;
	current = env_list;
	count = 0;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		env_array[count] = ft_strjoin(tmp, current->value);
		free(tmp);
		count++;
		current = current->next;
	}
	env_array[count] = NULL;
	return env_array;
}

char *find_executable(char *cmd, t_env *env_list)
{
	char *path_var;
	char **paths;
	char *full_path;
	char *tmp;
	int i;

	if (ft_strchr(cmd, '/'))
		return ft_strdup(cmd);
	path_var = get_env_value(env_list, "PATH");
	if (!path_var)
		return NULL;
	paths = ft_split(path_var, ':');
	if (!paths)
		return NULL;
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_2d_string(paths);
			return full_path;
		}
		free(full_path);
		i++;
	}
	free_2d_string(paths);
	return (NULL);
}

cmd_status execute_external(t_ast_node *cmd_node, t_shell *shell)
{
	pid_t pid;
	int status;
	char **args;
	char *cmd_path;
	char **env_array;
	cmd_status ret;

	if ((pid = fork()) == -1)
	{
		perror("fork in execute_external");
		return CMD_ERROR;
	}
	if (pid == 0)
	{
		args = get_command_arguments(cmd_node);
		if (!args)
			exit(CMD_ERROR);
		cmd_path = find_executable(args[0], shell->env_list);
		if (!cmd_path)
		{
			printf("command not found: %s\n", args[0]);
			exit(CMD_ERROR);
		}
		env_array = create_env_array(shell->env_list);
		if (!env_array)
			exit(CMD_ERROR);
		execve(cmd_path, args, env_array);
		perror("execve");
		free(cmd_path);
		free_2d_string(env_array);
		free_2d_string(args);
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