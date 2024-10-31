/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 00:51:30 by candrese          #+#    #+#             */
/*   Updated: 2024/10/31 06:24:56 by candrese         ###   ########.fr       */
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

// Modify get_token_type to better handle command arguments
ast_type get_token_type(const char *data, const t_token *prev_token)
{
	ast_type basic_type;

	basic_type = get_basic_token_type(data[0]);
		
	// If it's already a special token, return that type
	if (basic_type != NODE_WORD)
		return basic_type;
	// If no previous token, or previous token was a pipe or redirection,
	// this word is a command
	if (!prev_token || prev_token->type == NODE_PIPE || 
		prev_token->type == NODE_REDIR)
		return NODE_CMD;
		
	// If previous token was a command or argument, this is an argument
	if (prev_token->type == NODE_CMD || prev_token->type == NODE_ARG)
		return NODE_ARG;
		
	return NODE_WORD;
}

ast_type get_basic_token_type(char first_char)
{
	if (first_char == '|')
		return NODE_PIPE;
	else if (first_char == '>' || first_char == '<')
		return NODE_REDIR;
	else if (first_char == '$')
		return NODE_ENV;
	else if (first_char == '"' || first_char == '\'')
		return NODE_QUOTE;
	else
		return NODE_WORD;
}
