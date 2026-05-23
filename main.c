/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:58:35 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/05/23 13:54:56 by zkarman          ###   ########.fr       */
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
			if (curr->outfile > 2)
				close(curr->outfile);
			if (curr->infile > 2)
				close(curr->infile);
			curr = curr->next;
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

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	shell;

	(void)argc;
	(void)argv;
	ini_shell(&shell, envp);
	while (1)
	{
		input = readline("minishell$ ");
		if (g_status == 130)
		{
			shell.exit_status = 130;
			g_status = 0;
		}
		if (!input)
		{
			/*if (shell.exit_status == 130)
			{
				shell.exit_status = 0;
				continue ;
			}*/
			write (1, "exit\n", 5);
			break ;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue;
		}
		add_history(input);
		process_input(&shell, input);
		free(input);
	}
	check_backups(&shell);
	if (shell.env_list)
		free_env(&(shell.env_list));
	rl_clear_history();
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	return (0);
}
