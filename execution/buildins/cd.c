/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:49:59 by stalash           #+#    #+#             */
/*   Updated: 2024/12/06 19:21:32 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Helper function to manage the previous directory
static char *get_and_update_prev_dir(const char *new_dir)
{
	static char *prev_dir = NULL;
	char *current_dir;

	// Store the current directory before updating
	if (new_dir)
	{
		current_dir = getcwd(NULL, 0);
		if (!current_dir)
		{
			perror("getcwd");
			return NULL;
		}
		// Free the old prev_dir and set it the new current dir
		if (prev_dir)
			free(prev_dir);
		prev_dir = ft_strdup(new_dir);
		free(current_dir);
	}
	return (prev_dir);
}

cmd_status ft_cd(t_ast_node *cmd_node)
{
	t_ast_node	*arg;
	char		*path;
	char		cwd[PATH_MAX];

	arg = cmd_node->args;
	// if no argument is provided, should change to HOME directory
	if (!arg)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
			return (CMD_ERROR);
		}
	}
	/* Handle "cd -" to switch to the previous directory
	 so the diferrence bettween cd .. and cd - is
	 cd - toggles between the current directory and the last directory you were in.
	 The shell maintains a memory of your last directory to support this functionality.
	 When executed, it also prints the path of the directory you switched to	*/
	else if (ft_strncmp(arg->data, "-", 2) == 0)
	{
		path = get_and_update_prev_dir(NULL);
		if (!path)
		{
			printf("cd: OLDPWD not set\n");
			return (CMD_ERROR);
		}
		// Print the previous directory
		printf("%s\n", path);
	}
	else
	{
		// Use the provided path
		path = arg->data;
		// Check if there are too many arguments
		if (arg->next)
		{
			printf("cd: too many arguments\n");
			return (CMD_ERROR);
		}
	}
	// Save the current working directory before changing
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		return CMD_ERROR;
	}
	// Try to change directory
	if (chdir(path) == -1)
	{
		// Print error messages
		if (access(path, F_OK) == -1)
			printf("cd: %s: No such file or directory\n", path);
		else if (access(path, R_OK) == -1)
			printf("cd: %s: Permission denied\n", path);
		else
			printf("cd: %s: Not a directory\n", path);
		return CMD_ERROR;
	}
	// if success updated prev_dir
	get_and_update_prev_dir(cwd);
	return (CMD_SUCCESS);
}

