/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 23:13:00 by candrese          #+#    #+#             */
/*   Updated: 2024/11/09 23:28:06 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

cmd_status ft_echo(t_ast_node *cmd_node)
{
	t_ast_node *arg;
	bool print_newline;

	print_newline = true;
	arg = cmd_node->args;
	if (arg && !ft_strncmp(arg->data, "-n", 3))
	{
		print_newline = false;
		arg = arg->next;
	}
	while (arg)
	{
		if (arg->data[0] == '"' || arg->data[0] == '\'')
			write(STDOUT_FILENO, arg->data + 1, ft_strlen(arg->data) - 2);
		else
			write(STDOUT_FILENO, arg->data, ft_strlen(arg->data));
		if (arg->next)
			write(STDOUT_FILENO, " ", 1);
		arg = arg->next;
	}
	if (print_newline)
		write(STDOUT_FILENO, "\n", 1);
	return CMD_SUCCESS;
}


