/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:01:49 by candrese          #+#    #+#             */
/*   Updated: 2024/11/08 14:01:59 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

// get_basic_token_type
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

// get_token_type to maybe change WORD NODE to CMD or ARG
ast_type get_token_type(const char *data, const t_token *prev_token)
{
	ast_type basic_type;

	// Check for environment variable
	if (data[0] == '$')
	{
		// If it's after a pipe or at the start, it should be a command
		if (!prev_token || prev_token->type == NODE_PIPE)
			return NODE_CMD;
		// Otherwise it's an argument
		return NODE_ARG;
	}
	basic_type = get_basic_token_type(data[0]);
	// If it's already a special token, return that type
	if (basic_type != NODE_WORD)
		return basic_type;
	// If no previous token, or previous token was a pipe, this word is a command
	if (!prev_token || prev_token->type == NODE_PIPE)
		return NODE_CMD;
	// If previous token was a redirection, this is a filename (WORD)
	if (prev_token->type == NODE_REDIR)
		return NODE_WORD;
	// If previous token was a command or argument, this is an argument
	if (prev_token->type == NODE_CMD || prev_token->type == NODE_ARG)
		return NODE_ARG;
	return NODE_WORD;
}

// Add token to list
void add_token(t_token **head, t_token *new_token)
{
	t_token *current;

	if (!*head)
	{
		*head = new_token;
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

void print_tokens(t_token *head)
{
	t_token *current = head;
	printf("Tokens:\n");
	while (current)
	{
		printf("Type: %d, Data: '%s'\n", current->type, current->data);
		current = current->next;
	}
	printf("\n");
}

// for Debug/Print
void print_ast(t_ast_node *node, int depth)
{
	int i;

	if (!node)
		return;

	for (i = 0; i < depth; i++)
		printf("  ");

	if(node -> type == NODE_PIPE)
		printf("PIPE\n");
	else if(node -> type == NODE_CMD)
		printf("CMD\n");
	else if(node -> type == NODE_REDIR)
		printf("REDIR\n");
	else if(node -> type == NODE_ARG)
		printf("ARG\n");
	else if(node -> type == NODE_WORD)
		printf("WORD\n");
	else if(node -> type == NODE_ENV)
		printf("ENV\n");
	else if(node -> type == NODE_QUOTE)
		printf("QUOTE\n");

	print_ast(node->args, depth + 1);
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}

