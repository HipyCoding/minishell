/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 07:14:15 by christian         #+#    #+#             */
/*   Updated: 2024/10/25 18:32:56 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <ctype.h>

bool is_space(char c)
{
    return (c == ' ' || c == '\t');
}

// // Utility to check if the token is a command (for simplicity, we assume the first non-whitespace token is a command)
// bool is_command(const char *token)
// {
//     // we can refine this function later to handle specific built-in commands or executables.
//     return true;  // Assuming it's a command if it's the first token
// }

// // Utility to check if the token is an argument (anything after the command)
// bool is_argument(const char *token) 
// {
//     // we refine this functino later
//     return true;  // All subsequent tokens after the command are arguments
// }
