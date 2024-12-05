/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:53:46 by stalash           #+#    #+#             */
/*   Updated: 2024/12/05 12:22:53 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// check if key exists in env list
t_env *find_env_var(t_env *env_list, const char *key)
{
	t_env *current;

	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}
static int count_env_nodes(t_env *env_list)
{
	t_env *current;
	int count;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static t_env **fill_env_array(t_env *env_list, int count)
{
	t_env **arr;
	t_env *current;
	int i;

	arr = ft_calloc(count, sizeof(t_env *));
	if (!arr)
		return (NULL);
	current = env_list;
	i = 0;
	while (current)
	{
		arr[i++] = current;
		current = current->next;
	}
	return (arr);
}

static void sort_env_array(t_env **arr, int count)
{
	int i;
	int j;
	t_env *temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
		if (ft_strcmp(arr[j]->key, arr[j + 1]->key) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void print_env_array(t_env **arr, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		printf("declare -x %s", arr[i]->key);
		if (arr[i]->value)
			printf("=\"%s\"", arr[i]->value);
		printf("\n");
		i++;
	}
}

void print_sorted_env(t_env *env_list)
{
	t_env **arr;
	int count;

	count = count_env_nodes(env_list);
	arr = fill_env_array(env_list, count);
	if (!arr)
		return;
	sort_env_array(arr, count);
	print_env_array(arr, count);
	free(arr);
}

static bool handle_key_value(char *key, char *value, t_env **env_list)
{
	t_env *existing;
	t_env *new_node;

	existing = find_env_var(*env_list, key);
	if (existing)
	{
		free(existing->value);
		existing->value = value;
		free(key);
		return (true);
	}
	new_node = create_env_node(key, value);
	if (!new_node)
	{
		free(key);
		free(value);
		return (false);
	}
	add_env_node(env_list, new_node);
	return (true);
}

static bool handle_key_only(char *arg, t_env **env_list)
{
	t_env *existing;
	t_env *new_node;

	existing = find_env_var(*env_list, arg);
	if (!existing)
	{
		new_node = create_env_node(ft_strdup(arg), NULL);
		if (!new_node)
			return (false);
		add_env_node(env_list, new_node);
	}
	return (true);
}

bool process_export_arg(char *arg, t_env **env_list)
{
	char *key;
	char *value;

	if (!ft_isalpha(arg[0]) && arg[0] != '_')
	{
		printf("export: '%s': not a valid identifier\n", arg);
		return (false);
	}
	if (split_env_str(arg, &key, &value))
		return (handle_key_value(key, value, env_list));
	return (handle_key_only(arg, env_list));
}

cmd_status ft_export(t_ast_node *cmd_node, t_env **env_list)
{
	t_ast_node *arg;
	// bool success;

	if (!cmd_node->args)
	{
		print_sorted_env(*env_list);
		return CMD_SUCCESS;
	}
	// handle arguments
	arg = cmd_node->args;
	while (arg)
	{
		if (!process_export_arg(arg->data, env_list))
			return (CMD_ERROR);
		arg = arg->next;
	}
	return (CMD_SUCCESS);
}
