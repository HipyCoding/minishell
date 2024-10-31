/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   tokenize.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: candrese <candrese@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/10/28 14:27:07 by candrese		  #+#	#+#			 */
/*   Updated: 2024/10/28 14:29:12 by candrese		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

# include "../minishell.h"

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

