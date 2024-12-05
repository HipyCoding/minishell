/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 19:20:09 by stalash           #+#    #+#             */
/*   Updated: 2024/12/05 12:04:07 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signal_handlers()
{
	signal(SIGINT, signal_handler); // for the "Ctrl-C"
	signal(SIGQUIT, SIG_IGN);		// for the "Ctrl- \"
	signal(SIGTSTP, SIG_IGN);		// and this one for the "Ctrl-Z"
}
