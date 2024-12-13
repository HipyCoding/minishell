/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 05:02:23 by candrese          #+#    #+#             */
/*   Updated: 2024/12/13 23:46:35 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast_node	*create_ast_node(ast_type type, char *data)
{
	t_ast_node	*node;

	node = ft_calloc(sizeof(t_ast_node), 1);
	if (!node)
		return (NULL);
	node->type = type;
	node->data = data;
	node->redir_type = 0;
	return (node);
}

t_token	*create_token(ast_type type, char *data)
{
	t_token	*new_token;

	new_token = ft_calloc(sizeof(t_token), 1);
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->data = data;
	return (new_token);
}

// Extract token data
char	*extract_token_data(const char *input, int *i)
{
	if (is_special_char(input[*i]))
		return (handle_special_char(input, i));
	return (handle_word(input, i));
}

// initialize new token
static	t_token	*create_new_token(char *token_data, t_token *prev_token)
{
	t_token		*new_token;
	ast_type	token_type;

	if (!token_data)
		return (NULL);
	token_type = get_token_type(token_data, prev_token);
	new_token = create_token(token_type, token_data);
	if (!new_token)
		free(token_data);
	return (new_token);
}

t_token	*init_new_token(const char *input, int *i, t_token *prev_token,
	t_shell *shell)
{
	char	*token_data;

	if (input[*i] == '$')
	{
		token_data = extract_env_var_name(input, i);
		return (create_new_token(token_data, prev_token));
	}
	else if (is_quote(input[*i]))
	{
		token_data = handle_quoted_string(input, i, shell);
		return (create_new_token(token_data, prev_token));
	}
	token_data = extract_token_data(input, i);
	return (create_new_token(token_data, prev_token));
}
