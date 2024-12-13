/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 23:39:30 by stalash           #+#    #+#             */
/*   Updated: 2024/12/14 00:04:30 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*handle_unclosed_quotes(char *input)
{
	char	*more_input;
	char	*temp;

	more_input = readline("dquote> ");
	if (!more_input)
	{
		free(input);
		return (NULL);
	}
	temp = ft_strjoin(input, "\n");
	free(input);
	input = ft_strjoin(temp, more_input);
	free(temp);
	free(more_input);
	return (input);
}

static t_token	*process_tokens(char *full_input, t_shell *shell)
{
	t_token	*head;
	t_token	*new_token;
	t_token	*prev_token;
	int		i;

	head = NULL;
	prev_token = NULL;
	i = 0;
	while (skip_whitespace(full_input, &i))
	{
		new_token = init_new_token(full_input, &i, prev_token, shell);
		if (!new_token)
		{
			cleanup_tokens(head);
			return (NULL);
		}
		add_token(&head, new_token);
		prev_token = new_token;
		i++;
	}
	return (head);
}

t_token	*lexer(const char *input, t_shell *shell)
{
	t_token	*head;
	char	*full_input;

	full_input = ft_strdup(input);
	while (has_unclosed_quotes(full_input))
	{
		full_input = handle_unclosed_quotes(full_input);
		if (!full_input)
			return (NULL);
	}
	head = process_tokens(full_input, shell);
	free(full_input);
	return (head);
}
