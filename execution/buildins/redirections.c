/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:09:40 by stalash           #+#    #+#             */
/*   Updated: 2024/11/28 21:12:41 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Redirect input
void	redirect_input(char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

// Redirect output
void	redirect_output(char *filename, bool append)
{
	int flags = O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC);
	int fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

// Handle heredoc
void	handle_heredoc(char *delimiter)
{
	int		pipefd[2];
	char	*line;

	pipe(pipefd);
	while (true)
	{
		line = readline("heredoc > ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
			break;
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}


cmd_status handle_redirection(t_ast_node *redir_node)
{
	int st_fd_out;
	int st_fd_in;
	cmd_status status;
	t_ast_node *cmd_node;

	// storing FDs
	st_fd_out = dup(STDOUT_FILENO);
	st_fd_in = dup(STDIN_FILENO);
	if (st_fd_out == -1 || st_fd_in == -1)
	{
		perror("backup FDs\n");
		return CMD_ERROR;
	}
	cmd_node = redir_node;
	while (cmd_node)
	{
		if (cmd_node->redir_type == 4) // <<
			handle_heredoc(cmd_node->right->data);
		cmd_node = cmd_node->left;
	}
	if (redir_node->redir_type == 2)
		redirect_input(redir_node->right->data);
	else if (redir_node->redir_type == 1)
		redirect_output(redir_node->right->data, false);
	else if (redir_node->redir_type == 3)
		redirect_output(redir_node->right->data, true);
	else if (redir_node->redir_type == 4)
		handle_heredoc(redir_node->right->data);
	// execute the command with redirections in place
	status = execute_ast(redir_node->left);
	// we reset filedscriptors back to standard here
	dup2(st_fd_out, STDOUT_FILENO);
	dup2(st_fd_in, STDIN_FILENO);
	close(st_fd_out);
	close(st_fd_in);
	return status;
}

// void	handle_redirection(t_ast_node *redir_node)
// {
// 	int st_fd_out = dup(STDOUT_FILENO);
// 	int st_fd_in = dup(STDIN_FILENO);
// 	printf("redir type: %d\n",redir_node->redir_type);
// 	if (redir_node->redir_type == 2)
// 		redirect_input(redir_node->right->data);
// 	else if (redir_node->redir_type == 1)
// 		redirect_output(redir_node->right->data, false); // False for truncate (overwrite)
// 	else if (redir_node->redir_type == 3)
// 		redirect_output(redir_node->right->data, true); // True for append
// 	else if (redir_node->redir_type == 4)
// 		handle_heredoc(redir_node->right->data);
// 	dup2(st_fd_out, STDOUT_FILENO);
// 	dup2(st_fd_in, STDIN_FILENO);


// }
