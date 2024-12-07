/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 07:07:12 by candrese          #+#    #+#             */
/*   Updated: 2024/12/07 16:41:45 by stalash          ###   ########.fr       */
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
char *expand_string(const char *str, t_shell *shell)
{
	if (str[0] == SINGLE_QUOTE_MARK[0])
		return ft_strdup(str + 1);
	if (ft_strcmp(str, "$?") == 0)
		return ft_itoa(shell->exit_status);
	if (str[0] == '$' && !ft_strchr(str + 1, '$'))
	{
		char *value = get_env_value(shell->env_list, str);
		if (value)
			return ft_strdup(value);
		return ft_strdup("");
	}
	return ft_strdup(str);
}

// Expand environment variables in the AST
void expand_env_vars_in_node(t_ast_node *node, t_shell *shell)
{
	char *expanded_value;
	t_ast_node *arg;

	if (!node)
		return;
	if (node->data && node->data[0] == '$')
	{
		expanded_value = expand_string(node->data, shell);
		if (expanded_value)
		{
			free(node->data);
			node->data = expanded_value;
		}
	}
	arg = node->args;
	while (arg)
	{
		if (arg && arg->data && arg->data[0] == '$')
		{
			expanded_value = expand_string(arg->data, shell);
			if (expanded_value)
			{
				free(arg->data);
				arg->data = expanded_value;
			}
		}
		arg = arg->next;
	}
	expand_env_vars_in_node(node->left, shell);
	expand_env_vars_in_node(node->right, shell);
}
