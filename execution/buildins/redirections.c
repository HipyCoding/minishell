/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:09:40 by stalash           #+#    #+#             */
/*   Updated: 2024/12/14 02:32:12 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// Redirect input
void	redirect_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
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
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
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
			break ;
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
}

static void	apply_redirections(t_ast_node *redir_node, t_ast_node *cmd_node)
{
	while (cmd_node)
	{
		if (cmd_node->redir_type == 4)
			handle_heredoc(cmd_node->right->data);
		cmd_node = cmd_node->left;
	}
	if (redir_node->redir_type == 2)
		redirect_input(redir_node->right->data);
	else if (redir_node->redir_type == 1)
		redirect_output(redir_node->right->data, false);
	else if (redir_node->redir_type == 3)
		redirect_output(redir_node->right->data, true);
}

cmd_status	handle_redirection(t_ast_node *redir_node, t_shell *shell)
{
	int			st_fd_out;
	int			st_fd_in;
	cmd_status	status;

	st_fd_out = dup(STDOUT_FILENO);
	st_fd_in = dup(STDIN_FILENO);
	if (st_fd_out == -1 || st_fd_in == -1)
		return (perror("backup FDs\n"), CMD_ERROR);
	apply_redirections(redir_node, redir_node);
	status = execute_ast(redir_node->left, shell);
	dup2(st_fd_out, STDOUT_FILENO);
	dup2(st_fd_in, STDIN_FILENO);
	close(st_fd_out);
	close(st_fd_in);
	return (status);
}
