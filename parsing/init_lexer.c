/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 05:02:23 by candrese          #+#    #+#             */
/*   Updated: 2024/12/13 17:58:51 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// AST node creation
t_ast_node *create_ast_node(ast_type type, char *data)
{
	t_ast_node *node;
	// static int node_count = 0;

	node = ft_calloc(sizeof(t_ast_node), 1);
	if (!node)
		return (NULL);
	node->type = type;
	node->data = data;
	node->redir_type = 0;
	return (node);
}

t_token *create_token(ast_type type, char *data)
{
	t_token *new_token;

	new_token = ft_calloc(sizeof(t_token), 1);
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->data = data;
	return (new_token);
}

// Extract token data
char *extract_token_data(const char *input, int *i)
{
	if (is_special_char(input[*i]))
		return handle_special_char(input, i);
	return handle_word(input, i);
}

// initialize new token
t_token *init_new_token(const char *input, int *i, t_token *prev_token, t_shell *shell)
{
	t_token *new_token;
	char *token_data;
	ast_type token_type;

	if (input[*i] == '$')
	{
		token_data = extract_env_var_name(input, i);
		if (!token_data)
			return NULL;
		token_type = get_token_type(token_data, prev_token);
		new_token = create_token(token_type, token_data);
		if (!new_token)
			free(token_data);
	}
	else if (is_quote(input[*i]))
	{
		token_data = handle_quoted_string(input, i, shell);
		if (!token_data)
			return NULL;
		token_type = get_token_type(token_data, prev_token);
		new_token = create_token(token_type, token_data);
		if (!new_token)
			free(token_data);
	}
	else
	{
		token_data = extract_token_data(input, i);
		if (!token_data)
			return NULL;
		new_token = create_token(get_token_type(token_data, prev_token), token_data);
		if (!new_token)
			free(token_data);
	}
	return new_token;
}

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

t_token	*lexer(const char *input, t_shell *shell)
{
	t_token	*head;
	t_token	*new_token;
	t_token	*prev_token;
	char	*full_input;
	int		i;

	full_input = ft_strdup(input);
	while (has_unclosed_quotes(full_input))
	{
		full_input = handle_unclosed_quotes(full_input);
		if (!full_input)
			return (NULL);
	}
	head = NULL;
	prev_token = NULL;
	i = 0;
	while (skip_whitespace(full_input, &i))
	{
		new_token = init_new_token(full_input, &i, prev_token, shell);
		if (!new_token)
		{
			cleanup_tokens(head);
			free(full_input);
			return (NULL);
		}
		add_token(&head, new_token);
		prev_token = new_token;
		i++;
	}
	free(full_input);
	return (head);
}
