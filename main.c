/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:08:23 by christian         #+#    #+#             */
/*   Updated: 2024/12/14 04:00:02 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static void	initialize_shell(t_shell *shell, char **envp)
{
	shell->env_list = init_env(envp);
	shell->exit_status = 0;
}

static int	process_input(t_shell *shell)
{
	char		*input;
	t_ast_node	*ast;
	t_token		*tokens;
	t_cmd_status	status;

	setup_signal_handlers();
	input = readline("minishell > ");
	if (!input)
	{
		write(STDOUT_FILENO, "exit\n", 6);
		return (0);
	}
	tokens = lexer(input, shell);
	if (!tokens)
		cleanup_tokens(tokens);

	ast = parse(tokens, NULL, shell, &status);
	if (ast)
	{
		status = execute_ast(ast, shell);
		free_ast(ast);
	}
	add_history(input);
	free(input);
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	initialize_shell(&shell, envp);
	if (!shell.env_list)
		return (1);
	while (process_input(&shell))
		;
	cleanup_env_list(shell.env_list);
	return (0);
}
