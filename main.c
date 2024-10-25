/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: christian <christian@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:08:23 by christian         #+#    #+#             */
/*   Updated: 2024/10/24 19:40:32 by christian        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//     void	*ft_calloc(size_t count, size_t size)
// {
// 	size_t	total;
// 	void	*ptr;

// 	total = count * size;
// 	ptr = malloc(total);
// 	if (!ptr)
//         return(NULL);
//     while (total > 0)
// 	{
// 		*ptr = 0;
// 		ptr++;
// 		total--;
// 	}
//     return (ptr);
// }

// void handle_sigint(int sig) 
// {
//     printf("\n");
//     rl_on_new_line();  // Inform readline that the user pressed Ctrl-C
//     rl_redisplay();  // Redisplay the prompt
// }

int main() 
{
    // Set up the signal handler for Ctrl-C
   // signal(SIGINT, handle_sigint);

    char *input;
    while (1) {
        input = readline("mini_shell > ");
        if (*input) {
            add_history(input);
        }

        printf("You entered: %s\n", input);

        free(input);
    }

    clear_history();
    return 0;
}