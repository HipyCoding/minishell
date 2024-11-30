/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:35:34 by stalash           #+#    #+#             */
/*   Updated: 2024/11/30 19:40:35 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Environment node creation
t_env *create_env_node(char *key, char *value)
{
	t_env *new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return NULL;
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return new_node;
}

// Split environment string into key and value
bool split_env_str(char *env_str, char **key, char **value)
{
	char *equals_sign;

	equals_sign = ft_strchr(env_str, '=');
	if (!equals_sign)
		return false;
	*equals_sign = '\0';
	*key = ft_strdup(env_str);
	*value = ft_strdup(equals_sign + 1);
	*equals_sign = '=';
	return (*key && *value);
}

// Add node to environment list
void add_env_node(t_env **env_list, t_env *new_node)
{
	new_node->next = *env_list;
	*env_list = new_node;
}

// Process single environment variable
t_env *process_env_var(char *env_str)
{
	char *key;
	char *value;
	t_env *new_node;

	if (!split_env_str(env_str, &key, &value))
		return NULL;
	new_node = create_env_node(key, value);
	if (!new_node)
	{
		free(key);
		free(value);
		return NULL;
	}
	return new_node;
}

// Initenvironment list
t_env *init_env(char **envp)
{
	t_env *env_list;
	t_env *new_node;
	int i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = process_env_var(envp[i]);
		if (!new_node)
			return NULL;
		add_env_node(&env_list, new_node);
		i++;
	}
	return env_list;
}

// Environment printing function
void print_env_list(t_env *env_list)
{
	if (env_list == NULL)
		return;
	print_env_list(env_list->next);
	if (env_list->value)
		printf("%s=%s\n", env_list->key, env_list->value);
}

// Check env command arguments
bool check_env_args(t_ast_node *cmd_node)
{
	if (cmd_node->args)
	{
		printf("env: too many arguments\n");
		return false;
	}
	return true;
}

// Environment builtin command
cmd_status ft_env(t_ast_node *cmd_node, t_env *env_list)
{
	if (!check_env_args(cmd_node))
		return CMD_ERROR;
	print_env_list(env_list);
	return CMD_SUCCESS;
}
