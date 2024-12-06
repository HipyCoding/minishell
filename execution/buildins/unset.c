/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:57:55 by stalash           #+#    #+#             */
/*   Updated: 2024/12/06 18:41:10 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Remove a node from the environment list by key
bool	remove_env_var(t_env **env_list, const char *key)
{
	t_env	*current;
	t_env	*prev;

	if (!env_list || !*env_list || !key)
		return (false);
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next; // Bypass the current node
			else
				*env_list = current->next; // Update the head of the list if first node
			free(current->key);
			free(current->value);
			free(current);
			return (true);
		}
		prev = current;
		current = current->next;
	}
	return (false); // Key not found
}

cmd_status	ft_unset(t_ast_node *cmd_node, t_env **env_list)
{
	t_ast_node	*arg;

	if (!cmd_node->args)
	{
		printf("unset: not enough arguments\n");
		return (CMD_ERROR);
	}
	arg = cmd_node->args;
	while (arg)
	{
		if (!ft_isalpha(arg->data[0]) && arg->data[0] != '_')
		{
			printf("unset: '%s': not a valid identifier\n", arg->data);
			return (CMD_ERROR);
		}
		remove_env_var(env_list, arg->data);
		arg = arg->next;
	}
	return (CMD_SUCCESS);
}
