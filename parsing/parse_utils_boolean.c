/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 07:14:15 by christian         #+#    #+#             */
/*   Updated: 2024/10/31 00:47:42 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


// Utility functions remain the same as before
bool is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$');
}

bool is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool is_quote(char c)
{
	return (c == '"' || c == '\'');
}

// Skip whitespace and check if end of input
bool skip_whitespace(const char *input, int *i)
{
	while (input[*i] && is_whitespace(input[*i]))
		(*i)++;
	return (input[*i] != '\0');
}
