/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:08:23 by christian         #+#    #+#             */
/*   Updated: 2024/10/25 18:33:00 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	handle_sig(int signal)
// {
// 	printf("'\n");				
// 	rl_on_new_line();			//inform readline about Ctrl-C
// 	rl_replace_line("", 0);		//clear current unput
// 	rl_redisplay();				//redisplay prompt
// }

int main()
{
	char *input;

	// signal(SIGINT, handle_sig);
	while(1)
	{
		input = readline("minishell > ");
		printf("input: %s\n", input);
		free(input);
	}
	return 0;
}
