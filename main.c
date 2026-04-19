/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:58:35 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/19 17:02:34 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	char *input;
	t_token *input_list;
	t_token	*temp;
	t_env	*env;
	int num;

	printf("Welcome to the Parsing Test!\n");
	printf("Type commands, use up/down arrows for history, or press Ctrl+D to exit.\n\n");
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
		input_list = NULL;
		env = build_evnp(&envp);
		if (build_token(&input_list, &env, input) == 1)
		{
			free_tokens(&input_list);
			free(input);
			continue;
		}
		num = 1;
		temp = input_list;
		while (temp != NULL)
		{
			printf("Token %i: [%s]\n", num, temp->value);
			temp = temp->next;
			num++;
		}
		free_tokens(&input_list);
		free(input);
	}
	return (0);
}
