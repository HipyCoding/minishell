/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:35:34 by stalash           #+#    #+#             */
/*   Updated: 2024/12/14 04:07:44 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Process single environment variable
t_env	*process_env_var(char *env_str)
{
	char	*key;
	char	*value;
	t_env	*new_node;

	if (!split_env_str(env_str, &key, &value))
		return (NULL);
	new_node = create_env_node(key, value);
	if (!new_node)
	{
		free(key);
		free(value);
		return (NULL);
	}
	return (new_node);
}

// Initenvironment list
t_env	*init_env(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = process_env_var(envp[i]);
		if (!new_node)
			return (NULL);
		add_env_node(&env_list, new_node);
		i++;
	}
	return (env_list);
}

// Environment printing function
void	print_env_list(t_env *env_list)
{
	if (env_list == NULL)
		return ;
	print_env_list(env_list->next);
	if (env_list->value)
		printf("%s=%s\n", env_list->key, env_list->value);
}

t_cmd_status	ft_env(t_ast_node *cmd_node, t_env *env_list)
{
	if (cmd_node->args)
		return (printf("env: too many arguments\n"), CMD_ERROR);
	print_env_list(env_list);
	return (CMD_SUCCESS);
}
