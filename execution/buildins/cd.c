/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:49:59 by stalash           #+#    #+#             */
/*   Updated: 2024/12/14 04:09:39 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*get_and_update_prev_dir(const char *new_dir)
{
	static char	*prev_dir = NULL;
	char		*current_dir;

	if (new_dir)
	{
		current_dir = getcwd(NULL, 0);
		if (!current_dir)
			return (perror("getcwd"), NULL);
		if (prev_dir)
			free(prev_dir);
		prev_dir = ft_strdup(new_dir);
		free(current_dir);
	}
	return (prev_dir);
}

t_cmd_status	validate_and_get_path(t_ast_node *arg, char **path)
{
	if (!arg)
	{
		*path = getenv("HOME");
		if (!*path)
			return (printf("cd: HOME not set\n"), CMD_ERROR);
	}
	else if (ft_strncmp(arg->data, "-", 2) == 0)
	{
		*path = get_and_update_prev_dir(NULL);
		if (!*path)
			return (printf("cd: OLDPWD not set\n"), CMD_ERROR);
		printf("%s\n", *path);
	}
	else
	{
		*path = arg->data;
		if (arg->next)
			return (printf("cd: too many arguments\n"), CMD_ERROR);
	}
	return (CMD_SUCCESS);
}

t_cmd_status	ft_cd(t_ast_node *cmd_node)
{
	t_ast_node	*arg;
	char		*path;
	char		cwd[PATH_MAX];

	arg = cmd_node->args;
	if (validate_and_get_path(arg, &path) == CMD_ERROR)
		return (CMD_ERROR);
	if (!getcwd(cwd, sizeof(cwd)))
		return (perror("getcwd"), CMD_ERROR);
	if (chdir(path) == -1)
	{
		if (access(path, F_OK) == -1)
			printf("cd: %s: No such file or directory\n", path);
		else if (access(path, R_OK) == -1)
			printf("cd: %s: Permission denied\n", path);
		else
			printf("cd: %s: Not a directory\n", path);
		return (CMD_ERROR);
	}
	get_and_update_prev_dir(cwd);
	return (CMD_SUCCESS);
}
