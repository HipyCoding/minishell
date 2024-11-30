/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 23:13:00 by candrese          #+#    #+#             */
/*   Updated: 2024/11/30 19:02:09 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


// TODO: single quotations and double quotations are not handled correctly
cmd_status ft_echo(t_ast_node *cmd_node)
{
	t_ast_node *arg;
	bool print_newline;

	print_newline = true;
	arg = cmd_node->args;
	// Handle -n flag
	while (arg && arg->data)
	{
		if (arg->data[0] != '-')
			break;
		char *ptr = arg->data + 1;
		while (*ptr == 'n')
			ptr++;
		if (*ptr == '\0')
		{
			print_newline = false;
			arg = arg->next;
		}
		else
			break;
	}
	// Print arguments
	while (arg)
	{
		if (arg->data)
		{
			// Check for single quote marker
			if (ft_strncmp(arg->data, SINGLE_QUOTE_MARK, ft_strlen(SINGLE_QUOTE_MARK)) == 0)
				write(STDOUT_FILENO, arg->data + ft_strlen(SINGLE_QUOTE_MARK),
					ft_strlen(arg->data) - ft_strlen(SINGLE_QUOTE_MARK));
			else
				write(STDOUT_FILENO, arg->data, ft_strlen(arg->data));
			if (arg->next)
				write(STDOUT_FILENO, " ", 1);
		}
		arg = arg->next;
	}
	if (print_newline)
		write(STDOUT_FILENO, "\n", 1);
	return CMD_SUCCESS;
}

