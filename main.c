/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:58:35 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/29 16:18:08 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ini_shell(t_shell *shell, char	**envp)
{
	shell->env_list = build_envp(envp);
	shell->input_list = NULL;
	shell->cmds = NULL;
	shell->exit_status = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
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

void process_input(t_shell *shell, char *input)
{
	if (build_token(shell, input) == 1)
		return ;
	if (syntax_checker(shell->input_list) == 1)
	{
		shell->exit_status = 2;
		free_tokens(&(shell.input_list));
		return ;
	}
	if (build_cmds(shell) == 1)
	{
		free_tokens(&(shell->input_list));
		return ;
	}
	reading_commands(shell);
	free_cmds(shell.cmds);
	shell.cmds = NULL;
	free_tokens(&(shell.input_list));
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	char	*input;
	t_shell shell;

	ini_shell(&shell, envp);
	while (1)
	{
		if (g_status != 0)
		{
			shell.exit_status = g_status;
			g_status = 0;
		}
		input = readline("minishell> ");
		if (input == NULL)
			break;
		if (input[0] != '\0')
			add_history(input);
		process_input(&shell, input);
		free(input);
	}
	printf("\nexit\n");
	free_env(shell.env_list);
	rl_clear_history();
	return (0);
}
