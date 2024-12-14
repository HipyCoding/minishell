/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 00:51:30 by candrese          #+#    #+#             */
/*   Updated: 2024/12/14 03:11:52 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_word_length(const char *s, int st, int l)
{
	char	quote;

	while (s[st + l] && !is_whitespace(s[st + l])
		&& !is_special_char(s[st + l]))
	{
		if (s[st + l] == '=')
		{
			l++;
			if (s[st + l] && (s[st + l] == '"' || s[st + l] == '\''))
			{
				quote = s[st + l];
				l++;
				while (s[st + l] && s[st + l] != quote)
					l++;
				l += (s[st + l] == quote);
			}
			else
				while (s[st + l] && !is_whitespace(s[st + l])
					&& !is_special_char(s[st + l]))
					l++;
			break ;
		}
		l++;
	}
	return (l);
}


// Helper function for special character tokens
char	*handle_special_char(const char *input, int *i)
{
	char	*data;

	if ((input[*i] == '>' && input[*i + 1] == '>')
		|| (input[*i] == '<' && input[*i + 1] == '<'))
	{
		data = ft_calloc(sizeof (char *), 3);
		if (!data)
			return (NULL);
		data[0] = input[*i];
		data[1] = input[*i + 1];
		data[2] = '\0';
		(*i)++;
	}
	else
	{
		data = ft_calloc(sizeof (char *), 2);
		if (!data)
			return (NULL);
		data[0] = input[*i];
		data[1] = '\0';
	}
	return (data);
}

// Handle word token extraction
char	*handle_word(const char *input, int *i)
{
	char	*data;
	int		len;
	int		start;

	start = *i;
	len = get_word_length(input, start, 0);
	data = ft_substr(input, start, len);
	if (!data)
		return (NULL);
	*i += len - 1;
	return (data);
}

static int	get_env_var_length(const char *input, int start)
{
	int	len;

	len = 0;
	while (input[start + len] && !is_whitespace(input[start + len])
		&& !is_special_char(input[start + len])
		&& !is_quote(input[start + len]))
		len++;
	return (len);
}

char	*extract_env_var_name(const char *input, int *i)
{
	int		start;
	char	*result;
	int		len;
	char	*var_name;

	start = *i + 1;
	if (!input[start] || is_whitespace(input[start])
		|| is_special_char(input[start]) || is_quote(input[start]))
		return (ft_strdup("$"));
	len = get_env_var_length(input, start);
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return (NULL);
	result = ft_calloc(len + 2, sizeof(char));
	if (!result)
		return (free(var_name), NULL);
	result[0] = '$';
	ft_strlcpy(result + 1, var_name, len + 1);
	free(var_name);
	*i += len;
	return (result);
}
