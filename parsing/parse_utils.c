/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: christian <christian@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 00:51:30 by candrese          #+#    #+#             */
/*   Updated: 2024/12/09 12:35:05 by christian        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int get_word_length(const char *input, int start)
{
	int len;

	len = 0;
	while (input[start + len] && !is_whitespace(input[start + len]) && 
			!is_special_char(input[start + len]))
	{
		if (input[start + len] == '=')
		{
			len++;
			if (input[start + len] && (input[start + len] == '"' || input[start + len] == '\''))
			{
				char quote = input[start + len];
				len++;
				while (input[start + len] && input[start + len] != quote)
					len++;
				if (input[start + len] == quote)
					len++;
			}
			else
				while (input[start + len] && !is_whitespace(input[start + len]) && 
					!is_special_char(input[start + len]))
				len++;
			break;
		}
		len++;
	}
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

char *handle_quoted_string(const char *input, int *i, t_shell *shell)
{
	char quote_char;
	int start;
	int len;
	char *data;
	char *result;
	char *var_value;

	quote_char = input[*i];
	start = *i + 1;
	len = 0;
	// For single quotes, keep existing behavior
	if (quote_char == '\'')
	{
		while (input[start + len] && input[start + len] != quote_char)
			len++;
		if (!input[start + len])
			return ft_strdup("\'");
		data = ft_substr(input + start, 0, len);
		if (!data)
			return NULL;
		result = ft_strjoin(SINGLE_QUOTE_MARK, data);
		free(data);
		*i += len + 1;
		return result;
	}
	// For double quotes handle variables
	result = ft_calloc(1, sizeof(char));
	while (input[start + len] && input[start + len] != quote_char)
	{
		if (input[start + len] == '$')
		{
			// Add everything before the $ to result
			if (len > 0)
			{
				data = ft_substr(input + start, 0, len);
				char *temp = ft_strjoin(result, data);
				free(result);
				free(data);
				result = temp;
			}
			// Get variable name length
			int var_len = 1;
			while (input[start + len + var_len] &&
					!is_whitespace(input[start + len + var_len]) &&
					input[start + len + var_len] != quote_char &&
					input[start + len + var_len] != '$')
				var_len++;
			// Get variable name without $
			data = ft_substr(input + start + len + 1, 0, var_len - 1);
			var_value = get_env_value(shell->env_list, data);
			free(data);
			// Add variable value to result
			if (var_value)
			{
				char *temp = ft_strjoin(result, var_value);
				free(result);
				result = temp;
			}
			start = start + len + var_len;
			len = 0;
		}
		else
			len++;
	}
	// Add remaining content
	if (len > 0)
	{
		data = ft_substr(input + start, 0, len);
		char *temp = ft_strjoin(result, data);
		free(result);
		free(data);
		result = temp;
	}
	*i += (start - (*i + 1)) + len + 1;
	return result;
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
