/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:58:35 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/05/17 17:56:18 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ini_shell(t_shell *shell, char	**envp)
{
	shell->env_list = build_envp(envp);
	shell->input_list = NULL;
	shell->cmds = NULL;
	shell->exit_status = 0;
	shell->pipe_processes = NULL;
	shell->backup_stdout = -1;
	shell->backup_stdin = -1;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

void	exit_program(t_shell *shell, int exit_code)
{
	if (shell)
	{
		if (shell->backup_stdin > 2)
		{
			close(shell->backup_stdin);
			shell->backup_stdin = -1;
		}
		if (shell->backup_stdout > 2)
		{
			close(shell->backup_stdout);
			shell->backup_stdout = -1;
		}
		if (shell->cmds)
		{
			if (shell->cmds->outfile > 2)
				close(shell->cmds->outfile);
			if (shell->cmds->infile > 2)
				close(shell->cmds->infile);
			free_cmds(&(shell->cmds));
		}
		if (shell->env_list)
			free_env(&(shell->env_list));
		if (shell->pipe_processes && shell->pipe_processes->children)
			free(shell->pipe_processes->children);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	exit (exit_code);
}

void	process_input(t_shell *shell, char *input)
{
	if (build_token(shell, input) == 1)
		return ;
	if (syntax_checker(shell->input_list) == 1)
	{
		shell->exit_status = 2;
		free_tokens(&(shell->input_list));
		return ;
	}
	if (build_cmds(shell) == 1)
	{
		free_tokens(&(shell->input_list));
		return ;
	}
	free_tokens(&(shell->input_list));
	shell->backup_stdin = -1;
	shell->backup_stdout = -1;
	reading_commands(shell);
	free_cmds(&(shell->cmds));
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	shell;

	(void)argc;
	(void)argv;
	ini_shell(&shell, envp);
	while (1)
	{
		if (g_status != 0)
		{
			shell.exit_status = g_status;
			g_status = 0;
		}
		input = readline("minishell$ ");
		if (input == NULL)
			break ;
		if (input[0] != '\0')
			add_history(input);
		process_input(&shell, input);
		free(input);
	}
	printf("exit\n");
	free_env(&(shell.env_list));
	rl_clear_history();
	return (0);
}
