/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/05 17:27:15 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_children(t_shell *shell, pid_t *child)
{
	int	i;
	int	status;

	i = 0;
	while (i < command_count(shell->cmds))
	{
		if (child[i] != -1)
		{
			if (waitpid(child[i], &status, 0) != -1)
			{
				if (WIFEXITED(status))
					shell->exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					shell->exit_status = 128 + WTERMSIG(status);
			}
		}
		i++;
	}
	free(child);
}

void	execute_command(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	**envp_arr;
	struct stat	path_stat;

	if (!cmd->args[0] || cmd->args[0][0] == '\0')
		exit_program(shell, 0);
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
		dup2(p->last_pipe, STDIN_FILENO);
	if (cmd->next)
		dup2(p->curr[1], STDOUT_FILENO);
	if (check_file_descriptors(cmd) == -1)
		exit_program(shell, 1);
	verifiy_stds(cmd);
	if (cmd->next)
	{
		close(p->curr[1]);
		close(p->curr[0]);
	}
	if (p->last_pipe != -1)
		close(p->last_pipe);
	if (is_built_in_command(cmd->args[0]))
	{
		shell->exit_status = execute_built_in_command(shell, cmd);
		exit_program(shell, shell->exit_status);
	}
	else
		execute_command(shell, cmd);
}

void	execute_system_command(t_shell *shell, t_cmd *curr, t_pipe *p)
{
	if (curr->next)
		pipe(p->curr);
	p->children[p->i] = fork();
	if (p->children[p->i] == 0)
		pipe_process(shell, curr, p);
	check_for_next_pipe(p, curr);
	close_if_non_standard_in_out_file(curr_cmd->infile, curr_cmd->outfile);
}

// Start of execution
void	reading_commands(t_shell *shell)
{
	t_pipe	p;
	t_cmd	*curr_cmd;
	int		stdout_dup;

	if (!shell->cmds)
		return ;
	p.last_pipe = -1;
	p.i = 0;
	initialize_children(shell, &p);
	if (!p.children)
		return ;
	check_heredocs(shell);
	curr_cmd = shell->cmds;
	stdout_dup = 0;
	loop_cmds(shell, &p, curr_cmd, stdout_dup);
	check_for_next_pipe(&p, NULL);
	wait_children(shell, p.children);
}
