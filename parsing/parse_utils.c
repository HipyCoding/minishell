/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 00:51:30 by candrese          #+#    #+#             */
/*   Updated: 2024/11/02 08:38:51 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Get length of word (ft_strlen but checks for whitespaces/special_chars)
int get_word_length(const char *input, int start)
{
	int len;

	len = 0;
	while (input[start + len] && !is_whitespace(input[start + len]) && 
			!is_special_char(input[start + len]))
		len++;
	return len;
}

// Helper function for special character tokens
char *handle_special_char(const char *input, int *i)
{
	char *data;
		
	if ((input[*i] == '>' && input[*i + 1] == '>') ||
		(input[*i] == '<' && input[*i + 1] == '<'))
	{
		data = ft_calloc(sizeof (char *), 3);
		if (!data)
			return NULL;
		data[0] = input[*i];
		data[1] = input[*i + 1];
		data[2] = '\0';
		(*i)++;
	}
	else
	{
		data = ft_calloc(sizeof (char*), 2);
		if (!data)
			return NULL;
		data[0] = input[*i];
		data[1] = '\0';
	}
	return data;
}

// Handle word token extraction
char *handle_word(const char *input, int *i)
{
	char *data;
	int len;
	int start;

	start = *i;
	len = get_word_length(input, start);
	data = ft_substr(input, start, len);
	if (!data)
		return NULL;
	*i += len - 1;
	return data;
}

char *extract_env_var_name(const char *input, int *i)
{
	int start;
	int len;
	char *var_name;
	char *result;

	start = *i + 1;
	// Check if there's no variable name after $
	if (!input[start] || is_whitespace(input[start]) || 
		is_special_char(input[start]) || is_quote(input[start]))
		return ft_strdup("$");
	len = 0;
	while (input[start + len] && !is_whitespace(input[start + len]) && 
		!is_special_char(input[start + len]) && !is_quote(input[start + len]))
		len++;
	var_name = ft_substr(input, start, len);
	if (!var_name)
		return NULL;
	// Create new string with $ prefix
	result = ft_calloc(len + 2, sizeof(char));
	if (!result)
	{
		free(var_name);
		return NULL;
	}
	result[0] = '$';
	ft_strlcpy(result + 1, var_name, len + 1);
	free(var_name);
	*i += len;
	return result;
}
