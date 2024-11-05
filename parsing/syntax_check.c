/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:34:11 by candrese          #+#    #+#             */
/*   Updated: 2024/11/05 15:10:25 by candrese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


// helper function to check if a command exists
bool is_valid_command(const char *cmd) 
{
	// skip if it starts with a hyphen (like -l)
	if (cmd[0] == '-')
		return false;
	// TODO: Check if builtin command, Check if executable in PATH, if it is a valid relative/absolute path
	return true;
}

// Check validity of a command node and its arguments
syntax_error_t check_command_syntax(t_ast_node *cmd_node)
{
	if (!cmd_node || !cmd_node->data)
		return ERR_CMD_NOT_FOUND;
	// Check if the command is valid
	if (!is_valid_command(cmd_node->data))
		return ERR_CMD_NOT_FOUND;
	return SYNTAX_OK;
}

// Check validity of a redirection node
syntax_error_t check_redirection_syntax(t_ast_node *redir_node)
{
	// Check if redirection has a target file
	if (!redir_node->right || !redir_node->right->data)
		return ERR_MISSING_REDIR_FILE;
	// Check if the redirection has a valid command
	if (!redir_node->left)
		return ERR_INVALID_REDIR;
	return SYNTAX_OK;
}

// Main syntax checker function
syntax_error_t check_syntax(t_ast_node *node)
{
	syntax_error_t error;

	if (!node)
		return ERR_CMD_NOT_FOUND;
	if (node->type == NODE_PIPE)
	{
		// Check for empty pipe or consecutive pipes
		if (!node->left || !node->right)
			return ERR_EMPTY_PIPE;
		// Recursively check both sides of the pipe
		error = check_syntax(node->left);
		if (error != SYNTAX_OK)
			return error;
		return check_syntax(node->right);
	}
	else if (node->type == NODE_CMD)
		return check_command_syntax(node);
	else if (node->type == NODE_REDIR)
	{
		error = check_redirection_syntax(node);
		if (error != SYNTAX_OK)
			return error;
		// Check the command part of redirection
		return check_syntax(node->left);
	}
	else
		return SYNTAX_OK;
}

// Function to display the right error message
void display_syntax_error(syntax_error_t error)
{
	if (error == SYNTAX_OK)
		return;
	else if (error == ERR_EMPTY_PIPE)
		ft_putstr_fd("syntax error: empty pipe\n", STDERR_FILENO);
	else if (error == ERR_INVALID_REDIR)
		ft_putstr_fd("syntax error: invalid redirection\n", STDERR_FILENO);
	else if (error == ERR_CMD_NOT_FOUND)
		ft_putstr_fd("command not found\n", STDERR_FILENO);
	else if (error == ERR_CONSECUTIVE_PIPE)
		ft_putstr_fd("syntax error: consecutive pipes\n", STDERR_FILENO);
	else if (error == ERR_MISSING_REDIR_FILE)
		ft_putstr_fd("syntax error: missing redirection file\n", STDERR_FILENO);
}
