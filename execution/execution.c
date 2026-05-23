/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/23 13:14:58 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_children(t_shell *shell, pid_t *child, int child_count)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < child_count)
	{
		if (child[i] >= 0)
		{
			waitpid(child[i], &status, 0);
			if (i == child_count - 1)
			{
				if (WIFEXITED(status))
					shell->exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
				{
					if (WTERMSIG(status) == SIGQUIT)
					{
						ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
						shell->exit_status = 131;
					}
					else if (WTERMSIG(status) == SIGINT)
					{
						ft_putstr_fd("\n", STDERR_FILENO);
						shell->exit_status = 130;
					}
				}
			}
		}
		i++;
	}
}

void	execute_command(t_shell *shell, t_cmd *cmd)
{
	char		*path;
	char		**envp_arr;
	struct stat	path_stat;

	if (!cmd->args[0])
		exit_program(shell, 0);
	if (cmd->args[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		exit_program(shell, 127);
	}
	envp_arr = envp_list_to_arr(shell);
	if (ft_strchr(cmd->args[0], '/'))
		path = ft_strdup(cmd->args[0]);
	else
		path = get_path(cmd->args[0], envp_arr);
	if (!path)
		exit_no_path(shell, envp_arr, cmd);
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		exit_no_access(shell, path, envp_arr);
	if (execve(path, cmd->args, envp_arr) == -1)
		exit_execve_failure(shell, envp_arr, cmd);
}

void	pipe_process(t_shell *shell, t_cmd *cmd, t_pipe *p)
{
	if (p->last_pipe != -1)
	{
		dup2(p->last_pipe, STDIN_FILENO);
		close(p->last_pipe);
		p->last_pipe = -1;
	}
	if (cmd->next)
	{
		dup2(p->curr[1], STDOUT_FILENO);
		close(p->curr[1]);
		close(p->curr[0]);
		p->curr[1] = -1;
		p->curr[0] = -1;
	}
	if (check_file_descriptors(cmd) == -1)
		exit_fd_failure(shell, cmd, p);
	verify_stds(cmd);
	if (is_built_in_command(cmd->args[0]))
	{
		shell->exit_status = execute_built_in_command(shell, cmd);
		if (p->curr[1] != -1)
			close(p->curr[1]);
		if (p->curr[0] != -1)
			close(p->curr[0]);
		kill_child(shell, shell->exit_status);
	}
	else
		execute_command(shell, cmd);
}

int	execute_system_command(t_shell *shell, t_cmd *curr, t_pipe *p)
{
	if (curr->next)
	{
		if (pipe(p->curr) == -1)
		{
			perror("minishell: pipe");
			shell->exit_status = 1;
			return (1);
		}
	}
	p->children[p->i] = fork();
	if (p->children[p->i] == -1)
	{
		perror("minishell: fork");
		shell->exit_status = 1;
		if (curr->next)
		{
			close(p->curr[1]);
			close(p->curr[0]);
		}
		if (p->last_pipe != -1)
		{
			close(p->last_pipe);
			p->last_pipe = -1;
		}
		return (1);
	}
	if (p->children[p->i] == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		pipe_process(shell, curr, p);
	}
	check_for_next_pipe(p, curr);
	close_if_non_standard_in_out_file(&curr->infile, &curr->outfile);
	return (0);
}

// Start of execution
void	reading_commands(t_shell *shell)
{
	t_pipe	p;
	t_cmd	*curr_cmd;
	int		stdout_dup;

	if (!shell->cmds)
		return ;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	p.last_pipe = -1;
	p.curr[0] = -1;
	p.curr[1] = -1;
	p.i = 0;
	shell->pipe_processes = &p;
	initialize_children(shell, &p);
	if (!p.children)
	{
		shell->pipe_processes = NULL;
		setup_prompt_signals();
		return ;
	}
	if (check_heredocs(shell))
	{
		shell->pipe_processes = NULL;
		setup_prompt_signals();
		free(p.children);
		return ;
	}
	curr_cmd = shell->cmds;
	stdout_dup = 0;
	if (loop_cmds(shell, &p, curr_cmd, stdout_dup) != 0)
	{
		if (p.last_pipe != -1)
			close(p.last_pipe);
		wait_children(shell, p.children, command_count(shell->cmds));
		free(p.children);
		shell->pipe_processes = NULL;
		setup_prompt_signals();
		return ;
	}
	check_for_next_pipe(&p, NULL);
	wait_children(shell, p.children, command_count(shell->cmds));
	free(p.children);
	shell->pipe_processes = NULL;
	setup_prompt_signals();
}
