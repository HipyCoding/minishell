/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_of_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 18:40:03 by stalash           #+#    #+#             */
/*   Updated: 2024/12/13 18:43:02 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Environment node creation
t_env	*create_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

bool	split_env_str(char *env_str, char **key, char **value)
{
	char	*equals_sign;
	char	*temp_value;

	equals_sign = ft_strchr(env_str, '=');
	if (!equals_sign)
		return (false);
	*equals_sign = '\0';
	*key = ft_strdup(env_str);
	*equals_sign = '=';
	temp_value = equals_sign + 1;
	if (temp_value[0] == '"' || temp_value[0] == '\'')
		*value = ft_substr(temp_value, 1, ft_strlen(temp_value) - 2);
	else
		*value = ft_strdup(temp_value);
	return (*key && *value);
}

// Add node to environment list
void	add_env_node(t_env **env_list, t_env *new_node)
{
	new_node->next = *env_list;
	*env_list = new_node;
}
