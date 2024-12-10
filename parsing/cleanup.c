/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: christian <christian@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 04:56:02 by candrese          #+#    #+#             */
/*   Updated: 2024/12/10 19:10:24 by christian        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void free_2d_string(char **array)
{
	int i;

	if (!array)
		return;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void cleanup_env_list(t_env *env_list)
{
	t_env *current;
	t_env *next;

	current = env_list;
	while (current)
	{
		next = current->next;
		if (current)
		{
			free(current->key);
			free(current->value);
			free(current);
		}
		current = next;
	}
}

void cleanup_tokens(t_token *head)
{
	t_token *temp;

	while (head)
	{
		temp = head->next;
		free(head->data);
		free(head);
		head = temp;
	}
}

void free_ast (t_ast_node *node)
{
	// static int freed_count = 0;
	if (!node)
		return;
	// Free all children first
	free_ast(node->left);
	free_ast(node->right);
	// Free argument list if it exists
	t_ast_node *current_arg = node->args;
	t_ast_node *next_arg;
	while (current_arg)
	{
		next_arg = current_arg->next;
		free(current_arg->data);
		free(current_arg);
		current_arg = next_arg;
	}
	// Free node -> data
	free(node->data);
	free(node);
}


// void free_ast(t_ast_node *node)
// {
// 	if (!node)
// 		return;

// 	free_ast(node->args);
// 	free_ast(node->left);
// 	free_ast(node->right);

// 	free(node->data);
// 	free(node);
// }

void free_tokens(t_token *head)
{
	t_token *current;
	t_token *next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
}
