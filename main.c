/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:58:35 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/28 15:31:27 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redirs(t_redir *redirs)
{
	t_redir	*tmp;

	tmp = redirs;
	if (!tmp)
	{
		printf("    (No redirections)\n");
		return ;
	}
	while (tmp != NULL)
	{
		// Note: You might need to print the raw integer for 'type' 
		// depending on how your TOKEN_IN, TOKEN_OUT enum is set up!
		printf("    [Redir] Type: %d, File: %s\n", tmp->type, tmp->file);
		tmp = tmp->next;
	}
}

void	print_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;
	int		j;

	tmp = cmds;
	i = 1;
	printf("\n========== PARSER DUMP ==========\n");
	while (tmp != NULL)
	{
		printf("CMD NODE %d:\n", i);
		printf("  -> Args:\n");
		j = 0;
		if (tmp->args && tmp->args[0])
		{
			while (tmp->args[j] != NULL)
			{
				printf("    [%d]: %s\n", j, tmp->args[j]);
				j++;
			}
		}
		else
			printf("    (No arguments)\n");
		
		printf("  -> Redirs:\n");
		print_redirs(tmp->redirs);
		
		printf("=================================\n");
		tmp = tmp->next;
		i++;
	}
	printf("\n");
}

void    exit_program(t_shell *shell, int exit_code)
{
    if (shell)
    {
        if (shell->cmds)
            free_cmds(shell->cmds);
        if (shell->env_list)
            free_env(shell->env_list);
    }
    exit (exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	char	*input;
	t_shell shell;

	printf("Welcome to the Parsing Test!\n");
	ini_shell(&shell, envp);
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("\nexit\n");
			free_env(shell.env_list);
			rl_clear_history();
			break;
		}
		if (input[0] != '\0')
			add_history(input);
		if (build_token(&shell, input) == 1)
		{
			free(input);
			continue;
		}
		if (build_cmds(&shell) == 1)
		{
			free_tokens(&(shell.input_list));
			free(input);
			continue;
		}
		//print_cmds(shell.cmds);
		reading_commands(&shell);
		free_cmds(shell.cmds);
		free_tokens(&(shell.input_list));
		free(input);
	}
	return (0);
}
