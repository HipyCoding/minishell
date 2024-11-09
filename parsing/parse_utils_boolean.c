/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_boolean.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 07:14:15 by christian         #+#    #+#             */
/*   Updated: 2024/11/08 14:42:04 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
