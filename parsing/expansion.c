/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: christian <christian@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 07:07:12 by candrese          #+#    #+#             */
/*   Updated: 2024/12/01 09:51:41 by christian        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

// Search for environment variable in the list
char *get_env_value(t_env *env_list, const char *key)
{
	t_env *current;

	if (!key)
		return NULL;
	if (key[0] == '$')
		key++;
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return current->value;
		current = current->next;
	}
	return NULL;
}

// Expand a single string that might contain environment variables
char *expand_string(const char *str, t_env *env_list)
{
	if (str[0] == SINGLE_QUOTE_MARK[0])
		return ft_strdup(str + 1);
	// If string starts with $ and contains no other special characters we
	// treat it as a environment variabel
	if (str[0] == '$' && !ft_strchr(str + 1, '$'))
	{
		char *value = get_env_value(env_list, str);
		if (value)
			return ft_strdup(value);
		return ft_strdup("");
	}
	return ft_strdup(str);
}

// Expand environment variables in the AST
void expand_env_vars_in_node(t_ast_node *node, t_env *env_list)
{
	char *expanded_value;
	t_ast_node *arg;

	if (!node)
		return;
	if (node->data && node->data[0] == '$')
	{
		expanded_value = expand_string(node->data, env_list);
		if (expanded_value)
		{
			free(node->data);
			node->data = expanded_value;
		}
	}
	arg = node->args;
	while (arg)
	{
		if (arg->data && arg->data[0] == '$')
		{
			expanded_value = expand_string(arg->data, env_list);
			if (expanded_value)
			{
				free(arg->data);
				arg->data = expanded_value;
			}
		}
		arg = arg->next;
	}
	// travers left and right nodes
	expand_env_vars_in_node(node->left, env_list);
	expand_env_vars_in_node(node->right, env_list);
}