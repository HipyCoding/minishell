/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 00:44:43 by stalash           #+#    #+#             */
/*   Updated: 2024/12/14 01:08:53 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*handle_single_quote(const char *input, int *i, int start, int len)
{
	char	*data;
	char	*result;

	data = ft_substr(input + start, 0, len);
	if (!data)
		return (NULL);
	result = ft_strjoin(SINGLE_QUOTE_MARK, data);
	free(data);
	*i += len + 1;
	return (result);
}

static char	*process_env_var(const char *input, int *s_len, t_shell *shell)
{
	char	*data;
	char	*value;
	int		var_len;
	int		start;

	start = s_len[0] + s_len[1];
	var_len = 1;
	while (input[start + var_len] && !is_whitespace(input[start + var_len])
		&& input[start + var_len] != '"' && input[start + var_len] != '$')
		var_len++;
	data = ft_substr(input + start + 1, 0, var_len - 1);
	value = get_env_value(shell->env_list, data);
	free(data);
	s_len[0] = s_len[0] + s_len[1] + var_len;
	s_len[1] = 0;
	return (ft_strdup(value));
}

static char	*process_double_quote_chunk(const char *input,
	int *s_len, char *result, t_shell *shell)
{
	char	*data;
	char	*temp;

	if (input[s_len[0] + s_len[1]] == '$')
	{
		if (s_len[1] > 0)
		{
			data = ft_substr(input + s_len[0], 0, s_len[1]);
			temp = ft_strjoin(result, data);
			free(result);
			free(data);
			result = temp;
		}
		data = process_env_var(input, s_len, shell);
		if (data)
		{
			temp = ft_strjoin(result, data);
			free(result);
			free(data);
			result = temp;
		}
	}
	else
		s_len[1]++;
	return (result);
}

static char	*handle_dquote_content(const char *input, int start, t_shell *shell)
{
	char	*data;
	char	*temp;
	int		len;
	int		s_len[2];
	char	*result;

	len = 0;
	result = ft_calloc(1, sizeof(char));
	s_len[0] = start;
	while (input[start + len] && input[start + len] != '"')
	{
		s_len[1] = len;
		result = process_double_quote_chunk(input, s_len, result, shell);
		len = s_len[1];
		start = s_len[0];
	}
	if (len > 0)
	{
		data = ft_substr(input + start, 0, len);
		temp = ft_strjoin(result, data);
		free(result);
		free(data);
		result = temp;
	}
	return (result);
}

char	*handle_quoted_string(const char *input, int *i, t_shell *shell)
{
	char	quote;
	int		start;
	int		len;
	char	*result;

	quote = input[*i];
	start = *i + 1;
	len = 0;
	while (input[start + len] && input[start + len] != quote)
		len++;
	if (!input[start + len])
		return (NULL);
	if (quote == '\'')
		return (handle_single_quote(input, i, start, len));
	result = handle_dquote_content(input, start, shell);
	*i += (start - (*i + 1)) + len + 1;
	return (result);
}
