/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: christian <christian@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:50:07 by christian         #+#    #+#             */
/*   Updated: 2024/09/21 16:40:05 by christian        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_shell
{
    char    *input;
    pid_t   *pids;
}   t_shell;

typedef struct s_token
{
    char    *type;
}   t_token;

#endif