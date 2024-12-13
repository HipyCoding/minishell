/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:08:23 by christian         #+#    #+#             */
/*   Updated: 2024/12/13 17:34:09 by stalash          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



// void	handle_sig(int signal)
// {
// 	printf("'\n");
// 	rl_on_new_line();			//inform readline about Ctrl-C
// 	rl_replace_line("", 0);		//clear current unput
// 	rl_redisplay();				//redisplay prompt
// }

// main with readline (no arguments)

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_shell shell;
	char *input;
	t_ast_node *ast;
	t_token *tokens;
	cmd_status status;

	shell.env_list = init_env(envp);
	shell.exit_status = 0;
	if (!shell.env_list)
		return 1;
	while (1)
	{
		setup_signal_handlers();
		input = readline("minishell > ");
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 6);
			break;
		}
		tokens = lexer(input, &shell);
		if (!tokens)
			cleanup_tokens(tokens);
		// print_tokens(tokens);
		ast = parse(tokens, ast, &shell, &status);
		if (ast)
		{
			status = execute_ast(ast, &shell);
			free_ast(ast);
		}
		add_history(input);
		free(input);
	}
	cleanup_env_list(shell.env_list);
	return (0);
}

// main for debugger (with arguments)

// int main(int ac, char **av, char **envp)
// {
// 	t_shell shell;
// 	char *input;
// 	t_ast_node *ast;
// 	t_token *tokens;
// 	cmd_status status;

// 	if (ac < 1)
// 		return 1;
// 	ast = NULL;
// 	shell.env_list = init_env(envp);
// 	shell.exit_status = 0;
// 	if (!shell.env_list)
// 		return 1;
// 		input = av[1];
// 		tokens = lexer(input);
// 		if (!tokens)
// 			free(tokens);
// 		ast = parse(tokens, ast, &shell, &status);
// 		if (ast)
// 		{
// 			status = execute_ast(ast, &shell);
// 			free_ast(ast);
// 		}
// 		// add_history(input);
// 		// free(input);

// 	cleanup_env_list(shell.env_list);
// 	return (0);
// }
