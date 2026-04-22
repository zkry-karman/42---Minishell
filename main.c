/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:58:35 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/22 22:24:23 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	char	*input;
	t_shell shell;
	t_token	*temp;
	int		i;

	printf("Welcome to the Parsing Test!\n");
	ini_shell(&shell, envp);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("\nexit\n");
			break;
		}
		if (input[0] != '\0')
			add_history(input);
		shell.input_list = NULL;
		if (build_token(&shell, input) == 1)
		{
			free_tokens(&(shell.input_list));
			free(input);
			continue;
		}
		build_cmd(&shell);
		i = 1;
		temp = shell.input_list;
		while (temp != NULL)
		{
			printf("Token %i: [%s]\n", i, temp->value);
			temp = temp->next;
			i++;
		}
		free_tokens(&(shell.input_list));
		free(input);
	}
	return (0);
}
