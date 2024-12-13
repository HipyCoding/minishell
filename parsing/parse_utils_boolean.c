/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils_boolean.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 07:14:15 by christian         #+#    #+#             */
/*   Updated: 2024/12/14 00:09:50 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	has_unclosed_quotes(const char *input)
{
	int		i;
	char	quote_char;
	bool	in_quotes;

	i = 0;
	in_quotes = false;
	while (input[i])
	{
		if ((input[i] == '\'' || input[i] == '"') && !in_quotes)
		{
			quote_char = input[i];
			in_quotes = true;
		}
		else if (in_quotes && input[i] == quote_char)
			in_quotes = false;
		i++;
	}
	return (in_quotes);
}

bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '$');
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

bool	skip_whitespace(const char *input, int *i)
{
	while (input[*i] && is_whitespace(input[*i]))
		(*i)++;
	return (input[*i] != '\0');
}
