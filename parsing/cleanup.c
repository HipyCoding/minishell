/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 04:56:02 by candrese          #+#    #+#             */
/*   Updated: 2024/12/14 03:37:30 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_2d_string(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	cleanup_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

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

void	cleanup_tokens(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head->next;
		free(head->data);
		free(head);
		head = temp;
	}
}

void	free_ast(t_ast_node *node)
{
	t_ast_node	*current_arg;
	t_ast_node	*next_arg;

	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	current_arg = node->args;
	while (current_arg)
	{
		next_arg = current_arg->next;
		free(current_arg->data);
		free(current_arg);
		current_arg = next_arg;
	}
	free(node->data);
	free(node);
}
