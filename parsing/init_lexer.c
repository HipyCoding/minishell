/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 05:02:23 by candrese          #+#    #+#             */
/*   Updated: 2024/11/09 17:02:58 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// AST node creation
t_ast_node *create_ast_node(ast_type type, char *data)
{
	t_ast_node *node;

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
t_token *init_new_token(const char *input, int *i, t_token *prev_token)
{
	t_token *new_token;
	char *token_data;
	ast_type token_type;

	if (input[*i] == '$')
	{
		token_data = extract_env_var_name(input, i);
		if (!token_data)
			return NULL;
		// Determine if this env var should be a command or argument
		token_type = get_token_type(token_data, prev_token);
		new_token = create_token(token_type, token_data);
	}
	else if (is_quote(input[*i]))
	{
		token_data = handle_quoted_string(input, i);
		if (!token_data)
			return NULL;
		token_type = get_token_type(token_data, prev_token);
		// after a command it should be parsed as argument
		if (prev_token && prev_token->type == NODE_CMD)
			token_type = NODE_ARG;
		new_token = create_token(token_type, token_data);
	}
	else
	{
		token_data = extract_token_data(input, i);
		if (!token_data)
			return NULL;
		new_token = create_token(get_token_type(token_data, prev_token), token_data);
	}
	if (!new_token)
	{
		free(token_data);
		return NULL;
	}
	return new_token;
}

// Main lexer function
t_token *lexer(const char *input)
{
	t_token *head;
	t_token *new_token;
	t_token *prev_token;
	int i;

	head = NULL;
	prev_token = NULL;
	i = 0;

	while (skip_whitespace(input, &i))
	{
		new_token = init_new_token(input, &i, prev_token);
		if (!new_token)
			return NULL;
		add_token(&head, new_token);
		prev_token = new_token;
		i++;
	}
	return head;
}
