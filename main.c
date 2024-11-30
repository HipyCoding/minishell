/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stalash <stalash@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:08:23 by christian         #+#    #+#             */
/*   Updated: 2024/11/30 19:26:59 by stalash          ###   ########.fr       */
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

// main with readline (no arguments)

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	char *input;
	t_shell shell;
	t_ast_node *ast;
	t_token *tokens;
	cmd_status status;

	ast = NULL;
	tokens = NULL;
	status = 0;
	// signal(SIGINT, handle_sig);
	shell.env_list = init_env(envp);
	if (!shell.env_list)
		return 1;
	while(1)
	{
		// setup_signal_handlers();
		input = readline("minishell > ");
		if (!input)
			break;
		tokens = lexer(input);
		if (!tokens)
		{
			free(input);
			return 1;
		}
	ast = parse(tokens, ast, &status);
	status = execute_ast(ast, &shell);
	free(input);
	cleanup_tokens(tokens);
	if (ast)
		free_ast(ast);
	}
	return 0;
}
// main for debugger (with arguments)

// int main(int argc, char **argv, char **env)
// {
// 	t_ast_node *ast;
// 	t_token *tokens;
// 	cmd_status *status;

// 	if (argc < 2)
// 		return 1;
// 	status = 0;
// 	ast = NULL;
// 	tokens = NULL;
// 	printf("\ninput: %s\n", argv[1]);
// 	tokens = lexer(argv[1]);
// 	if (!tokens)
// 		return 1;
// 	ast = parse(tokens, ast, status);
// 	cleanup_tokens(tokens);
// 	if (ast)
// 		free_ast(ast);
// 	return 0;
// }
