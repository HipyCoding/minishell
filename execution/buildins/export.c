/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: christian <christian@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:53:46 by stalash           #+#    #+#             */
/*   Updated: 2024/12/09 12:33:04 by christian        ###   ########.fr       */
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

bool process_export_arg(char *arg, t_ast_node *current, t_shell *shell)
{
	char *key;
	char *value;

	int len = ft_strlen(arg);
	if (len > 0 && arg[len - 1] == '=' && current->next)
	{
		arg[len - 1] = '\0';
		key = ft_strdup(arg);
		arg[len - 1] = '=';
		value = ft_strdup(current->next->data);
		return handle_key_value(key, value, &shell->env_list);
	}
	if (split_env_str(arg, &key, &value))
		return handle_key_value(key, value, &shell->env_list);
	return handle_key_only(arg, &shell->env_list);
}
cmd_status ft_export(t_ast_node *cmd_node, t_shell *shell)
{
	t_ast_node *arg;

	if (!cmd_node->args)
	{
		print_sorted_env(shell->env_list);
		return CMD_SUCCESS;
    }
	arg = cmd_node->args;
	while (arg)
	{
		if (!process_export_arg(arg->data, arg, shell))
			return CMD_ERROR;
		// Skip next arg if we used it as a value
		if (arg->next && ft_strlen(arg->data) > 0 && 
			arg->data[ft_strlen(arg->data) - 1] == '=')
			arg = arg->next;
		arg = arg->next;
	}
	return CMD_SUCCESS;
}
