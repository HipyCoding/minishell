/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:02:07 by stalash           #+#    #+#             */
/*   Updated: 2024/12/04 12:42:28 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int create_pipe(int pipefd[2])
{
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    return (0);
}

int fork_left_process(int pipefd[2], t_ast_node *node, t_shell *shell)
{
    pid_t 		pid;
	cmd_status ret;

	pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (-1);
    }
    if (pid == 0)
    {
        // In child process: left side of the pipeline
        close(pipefd[0]); // Close the read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write end
        close(pipefd[1]); // Close the write end of the pipe after redirect

        ret = execute_ast(node->left, shell);  // Execute left command
        if (ret == CMD_SUCCESS)
            exit(0);
        else
            exit(1);
    }
    return (pid);
}

int fork_right_process(int pipefd[2], t_ast_node *node, t_shell *shell)
{
    pid_t 		pid;
	cmd_status ret;

	pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (-1);
    }
    if (pid == 0)
    {
        // In child process: right side of the pipeline
        close(pipefd[1]); // Close the write end of the pipe
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read end
        close(pipefd[0]); // Close the read end of the pipe after redirect

        ret = execute_ast(node->right, shell);  // Execute right command
        if (ret == CMD_SUCCESS)
            exit(0);
        else
            exit(1);
    }
    return (pid);
}

cmd_status wait_for_children(pid_t pid1, pid_t pid2)
{
    int status1;
	int	status2;

    // Wait for the first child process
    waitpid(pid1, &status1, 0);
    if (!WIFEXITED(status1))
        return (CMD_ERROR);

    // Wait for the second child process
    waitpid(pid2, &status2, 0);
    if (!WIFEXITED(status2))
        return (CMD_ERROR);

    return (CMD_SUCCESS);
}

cmd_status execute_pipeline(t_ast_node *node, t_shell *shell)
{
    int pipefd[2];
    pid_t pid1;
	pid_t pid2;

    if (create_pipe(pipefd) == -1)
        return (CMD_ERROR);
    pid1 = fork_left_process(pipefd, node, shell);
    if (pid1 == -1)
        return (CMD_ERROR);
    pid2 = fork_right_process(pipefd, node, shell);
    if (pid2 == -1)
        return (CMD_ERROR);

    // Close pipe file descriptors in parent
    close(pipefd[0]);
    close(pipefd[1]);
    return (wait_for_children(pid1, pid2));
}
