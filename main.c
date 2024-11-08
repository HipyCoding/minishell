/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: candrese <candrese@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 16:08:23 by christian         #+#    #+#             */
/*   Updated: 2024/11/05 13:39:34 by candrese         ###   ########.fr       */
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

// int main()
// {
// 	char *input;

// 	// signal(SIGINT, handle_sig);
// 	while(1)
// 	{

// 	input = readline("minishell > ");
// 	t_token *tokens = lexer(input);
// 	if (!tokens)
// 		return 1;
		
// 	t_ast_node *ast = parse(tokens);
// 	if (ast)
// 		print_ast(ast, 0);
	
// 	cleanup_tokens(tokens);
// 	if (ast)
// 		free_ast(ast);
// 	}
// 	return 0;
// }

// main for debugger (with arguments)

int main(int argc, char **argv)
{
	t_ast_node *ast;
	t_token *tokens;

	if (argc < 2)
		return 1;
	ast = NULL;
	tokens = NULL;
	printf("\ninput: %s\n", argv[1]);
	tokens = lexer(argv[1]);
	if (!tokens)
		return 1;
	ast = parse(tokens, ast);



	cleanup_tokens(tokens);
	if (ast)
		free_ast(ast);
	return 0;
}
