/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: christian <christian@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:50:07 by christian         #+#    #+#             */
/*   Updated: 2024/10/24 19:16:00 by christian        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H


# include <stdio.h>
//# include "../lib/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum
{
    BUILTIN,        // built-in commands e.g. echo, cd, pwd
    COMMAND,        // command for executables e.g. cd ls echo
    ARGUMENT,       // argument passed to a command (not a meta-character) e.g. -l, /home/user
    REDIRECTION,    // meta-characters for redirections < > << >>
                    // maybe we will need seperate types for each (ridirect in & out etc)
    PIPE,           // pipe | to redirect output of one command to input of next one
    ENV_VAR,        // environment variable prefixed with $ e.g. $HOME $USER
    QUOTE,          // '...' treat everything inside as literal. "..." similar but allows $-expansion
    DELIMITER,      // used for handling the heredoc (<<) e.g. EOF
    CONTROL,        // control operators ctrl-c ctrl-d
}       n_type;

typedef struct s_node
{
    n_type          node_type;
    void            *data;
    struct s_node   *left;
    struct s_node   *right;
    struct s_node   *next;
}       t_node;


#endif