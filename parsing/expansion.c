/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 07:07:12 by candrese          #+#    #+#             */
/*   Updated: 2024/12/13 18:27:44 by stalash          ###   ########.fr       */
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
static char	*expand_variables_in_string(const char *str, t_shell *shell)
{
	char	*result;
	int		i;

	i = 0;
	if (str[i] == '$')
		return (expand_string(str, shell));
	while (str[i] && str[i] != '$')
		i++;
	if (!str[i])
		return (ft_strdup(str));
	result = ft_substr(str, 0, i);
	if (!result)
		return (NULL);
	if (str[i] == '$')
	{
		char *expanded = expand_string(str + i, shell);
		if (!expanded)
		{
			free(result);
			return (NULL);
		}
		char *temp = ft_strjoin(result, expanded);
		free(result);
		free(expanded);
		return (temp);
	}
	return (result);
}

void	expand_env_vars_in_node(t_ast_node *node, t_shell *shell)
{
	t_ast_node	*arg;
	char		*expanded_value;

	if (!node)
		return ;
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
		if (arg->data && ft_strncmp(arg->data, SINGLE_QUOTE_MARK,
			ft_strlen(SINGLE_QUOTE_MARK)) != 0)
		{
			expanded_value = expand_variables_in_string(arg->data, shell);
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
