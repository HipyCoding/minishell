/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:53:46 by stalash           #+#    #+#             */
/*   Updated: 2024/12/14 04:08:16 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	print_sorted_env(t_env *env_list)
{
	t_env	**arr;
	int		count;

	count = count_env_nodes(env_list);
	arr = fill_env_array(env_list, count);
	if (!arr)
		return ;
	sort_env_array(arr, count);
	print_env_array(arr, count);
	free(arr);
}

static bool	handle_key_value(char *key, char *value, t_env **env_list)
{
	t_env	*existing;
	t_env	*new_node;

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

static bool	handle_key_only(char *arg, t_env **env_list)
{
	t_env	*existing;
	t_env	*new_node;

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

bool	process_export_arg(char *arg, t_ast_node *current, t_shell *shell)
{
	char	*key;
	char	*value;
	int		len;

	len = ft_strlen(arg);
	if (len > 0 && arg[len - 1] == '=' && current->next)
	{
		arg[len - 1] = '\0';
		key = ft_strdup(arg);
		arg[len - 1] = '=';
		value = ft_strdup(current->next->data);
		return (handle_key_value(key, value, &shell->env_list));
	}
	if (split_env_str(arg, &key, &value))
		return (handle_key_value(key, value, &shell->env_list));
	return (handle_key_only(arg, &shell->env_list));
}

t_cmd_status	ft_export(t_ast_node *cmd_node, t_shell *shell)
{
	t_ast_node	*arg;

	if (!cmd_node->args)
	{
		print_sorted_env(shell->env_list);
		return (CMD_SUCCESS);
	}
	arg = cmd_node->args;
	while (arg)
	{
		if (!process_export_arg(arg->data, arg, shell))
			return (CMD_ERROR);
		if (arg->next && ft_strlen(arg->data) > 0 \
			&& arg->data[ft_strlen(arg->data) - 1] == '=')
			arg = arg->next;
		arg = arg->next;
	}
	return (CMD_SUCCESS);
}
