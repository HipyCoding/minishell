/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:49:59 by stalash           #+#    #+#             */
/*   Updated: 2024/11/14 04:43:14 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

cmd_status ft_cd(t_ast_node *cmd_node)
{
	char *path;
	t_ast_node *arg;

	// get first argument
	arg = cmd_node->args;
	// if no argument is provided, should change to HOME directory
	if (!arg)
	{
		path = getenv("HOME");
		if (!path)
		{
			printf("cd: HOME not set\n");
			return CMD_ERROR;
		}
	}
	else
	{
		// Use the provided path
		path = arg->data;
		// Check if there are too many arguments
		if (arg->next)
		{
			printf("cd: too many arguments\n");
			return CMD_ERROR;
		}
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
	return CMD_SUCCESS;
}
