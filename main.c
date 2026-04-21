/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:58:35 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/20 15:18:29 by kzhu@student.42.f###   ########.fr       */
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
