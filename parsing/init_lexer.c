/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 05:02:23 by candrese          #+#    #+#             */
/*   Updated: 2024/11/02 08:33:46 by candrese         ###   ########.fr       */
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

// Keep the original create_token function
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

// Extract token data with type handling
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

	token_data = extract_token_data(input, i);
	if (!token_data)
		return NULL;

	new_token = create_token(get_token_type(token_data, prev_token), token_data);
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
