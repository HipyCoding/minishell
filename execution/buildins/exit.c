/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 13:01:03 by stalash           #+#    #+#             */
/*   Updated: 2024/12/06 19:21:06 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Check if the argument is valid or if there are too many arguments
static int check_arguments_of_exit(t_ast_node *arg_node)
{
	if (arg_node->next)
	{
		printf(BRIGHT_RED "stderr: exit: too many arguments\n" RESET_COLOR);
		return (CMD_ERROR);
	}
	return (0);
}

// Parse the exit status from the argument, return CMD_ERROR if invalid
static int parse_exit_status(char *arg)
{
	int i;

	i = -1;
	while (arg[++i])
	{
		// Allow negative numbers
		if (!ft_isdigit(arg[i]) && !(i == 0 && arg[i] == '-'))
		{
			printf(BRIGHT_RED "stderr: exit %s: \
					numeric argument required\n" RESET_COLOR,
				   arg);
			return (CMD_ERROR);
		}
	}
	return (ft_atoi(arg));
}

static void cleanup_resources(t_shell *shell, t_ast_node *cmd_node)
{
	cleanup_env_list(shell->env_list);
	free_ast(cmd_node);
}

cmd_status ft_exit(t_ast_node *cmd_node, t_shell *shell)
{
	int exit_status;
	t_ast_node *arg_node;

	exit_status = 0;
	arg_node = cmd_node->args;
	// Check arguments for validity
	if (arg_node && check_arguments_of_exit(arg_node) == CMD_ERROR)
		return (CMD_ERROR);

	// If argument exists, parse it
	if (arg_node)
	{
		exit_status = parse_exit_status(arg_node->data);
		if (exit_status == CMD_ERROR)
			return (CMD_ERROR);
	}
	cleanup_resources(shell, cmd_node);

	// Print exit message and exit with the status
	printf("exit\n");
	exit(exit_status);
}
