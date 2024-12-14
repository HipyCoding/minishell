/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 23:13:00 by candrese          #+#    #+#             */
/*   Updated: 2024/12/14 04:07:19 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

t_cmd_status	handle_flags_and_get_args(t_ast_node **arg, bool *print_newline)
{
	char	*ptr;

	*print_newline = true;
	while (*arg && (*arg)->data)
	{
		if ((*arg)->data[0] != '-')
			break ;
		ptr = (*arg)->data + 1;
		while (*ptr == 'n')
			ptr++;
		if (*ptr == '\0')
		{
			*print_newline = false;
			*arg = (*arg)->next;
		}
		else
			break ;
	}
	return (CMD_SUCCESS);
}

t_cmd_status	handle_and_print_args(t_ast_node *arg, bool print_newline)
{
	while (arg)
	{
		if (arg->data)
		{
			if (ft_strncmp(arg->data, SINGLE_QUOTE_MARK, \
						ft_strlen(SINGLE_QUOTE_MARK)) == 0)
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
	return (CMD_SUCCESS);
}

t_cmd_status	ft_echo(t_ast_node *cmd_node)
{
	t_ast_node	*arg;
	bool		print_newline;

	arg = cmd_node->args;
	if (handle_flags_and_get_args(&arg, &print_newline) == CMD_ERROR)
		return (CMD_ERROR);
	return (handle_and_print_args(arg, print_newline));
}
