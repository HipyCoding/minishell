/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 19:17:29 by christian         #+#    #+#             */
/*   Updated: 2024/12/13 22:07:32 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**get_command_arguments(t_ast_node *cmd_node)
{
	int			argc;
	t_ast_node	*arg_node;
	char		**args;
	int			i;

	argc = 0;
	i = 0;
	arg_node = cmd_node->args;
	while (arg_node)
	{
		argc++;
		arg_node = arg_node->next;
	}
	args = (char **)malloc((argc + 2) * sizeof(char *));
	if (!args)
		return (perror("malloc"), NULL);
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

char	**create_env_array(t_env *env_list)
{
	t_env	*current;
	char	**env_array;
	char	*tmp;
	int		i;
	int		count;

	count = 0;
	current = env_list;
	while (current && ++count)
		current = current->next;
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	current = env_list;
	i = 0;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	*find_executable(char *cmd, t_env *env_list)
{
	char	*path_var;
	char	**paths;
	char	*full_path;
	char	*tmp;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_env_value(env_list, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, X_OK) == 0)
			return (free_2d_string(paths), full_path);
		free(full_path);
	}
	return (free_2d_string(paths), NULL);
}

static void	prepare_and_execute_command(t_ast_node *cmd_node, t_shell *shell)
{
	char	**args;
	char	**env_array;
	char	*cmd_path;

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

cmd_status	execute_external(t_ast_node *cmd_node, t_shell *shell)
{
	pid_t		pid;
	int			status;
	cmd_status	ret;

	pid = fork();
	if (pid == -1)
		return (perror("fork in execute_external"), CMD_ERROR);
	if (pid == 0)
		prepare_and_execute_command(cmd_node, shell);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	else
		ret = CMD_ERROR;
	return (ret);
}
