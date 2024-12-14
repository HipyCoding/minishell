/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:02:07 by stalash           #+#    #+#             */
/*   Updated: 2024/12/14 04:08:47 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	create_pipe(int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	return (0);
}

int	fork_left_process(int pipefd[2], t_ast_node *node, t_shell *shell)
{
	pid_t		pid;
	t_cmd_status	ret;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		ret = execute_ast(node->left, shell);
		if (ret == CMD_SUCCESS)
			exit(0);
		else
			exit(1);
	}
	return (pid);
}

int	fork_right_process(int pipefd[2], t_ast_node *node, t_shell *shell)
{
	pid_t		pid;
	t_cmd_status	ret;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		ret = execute_ast(node->right, shell);
		if (ret == CMD_SUCCESS)
			exit(0);
		else
			exit(1);
	}
	return (pid);
}

t_cmd_status	wait_for_children(pid_t pid1, pid_t pid2)
{
	int	status1;
	int	status2;

	waitpid(pid1, &status1, 0);
	if (!WIFEXITED(status1))
		return (CMD_ERROR);
	waitpid(pid2, &status2, 0);
	if (!WIFEXITED(status2))
		return (CMD_ERROR);
	return (CMD_SUCCESS);
}

t_cmd_status	execute_pipeline(t_ast_node *node, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (create_pipe(pipefd) == -1)
		return (CMD_ERROR);
	pid1 = fork_left_process(pipefd, node, shell);
	if (pid1 == -1)
		return (CMD_ERROR);
	pid2 = fork_right_process(pipefd, node, shell);
	if (pid2 == -1)
		return (CMD_ERROR);
	close(pipefd[0]);
	close(pipefd[1]);
	return (wait_for_children(pid1, pid2));
}
