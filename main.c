/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:58:35 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/05/23 16:54:00 by kzhu@student.42.f###   ########.fr       */
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
	shell->backup_stdout = dup(STDOUT_FILENO);
	shell->backup_stdin = dup(STDIN_FILENO);
	rl_catch_signals = 0;
	setup_prompt_signals();
}

void	exit_program(t_shell *shell, int exit_code)
{
	t_cmd	*curr;

	if (shell)
	{
		check_backups(shell);
		curr = shell->cmds;
		while (curr)
		{
			close_if_non_standard_in_out_file(&curr->infile, &curr->outfile);
			curr = curr->next;
		}
		if (shell->cmds)
			free_cmds(&(shell->cmds));
		if (shell->env_list)
			free_env(&(shell->env_list));
		if (shell->pipe_processes && shell->pipe_processes->children)
			free(shell->pipe_processes->children);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
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
	reading_commands(shell);
	free_cmds(&(shell->cmds));
}

void	shell_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell$ ");
		if (g_status == 130)
		{
			shell->exit_status = 130;
			g_status = 0;
		}
		if (!input)
		{
			write (1, "exit\n", 5);
			break ;
		}
		if (input[0] != '\0')
		{
			add_history(input);
			process_input(shell, input);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	ini_shell(&shell, envp);
	shell_loop(&shell);
	main_cleanup(&shell);
	return (0);
}
