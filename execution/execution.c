/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/05 16:35:53 by zkarman          ###   ########.fr       */
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
	int		err_no;

	if (!cmd->args[0] || cmd->args[0][0] == '\0')
		exit_program(shell, 0);
	envp_arr = envp_list_to_arr(shell);
	if (ft_strchr(cmd->args[0], '/'))
		path = ft_strdup(cmd->args[0]);
	else
		path = get_path(cmd->args[0], envp_arr);
	if (!path)
	{
		free_array(envp_arr);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit_program(shell, 127);
	}
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit_program(shell, 126);
	}
	if (execve(path, cmd->args, envp_arr) == -1)
	{
		err_no = errno; 
		free_array(envp_arr);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(err_no), 2);
		ft_putstr_fd("\n", 2);
		if (err_no == ENOENT)
			exit_program(shell, 127);
		exit_program(shell, 126);
	}
}

void	pipe_process(t_shell *shell, t_cmd *cmd, t_pipe *p)
{
	if (p->last_pipe != -1)
		dup2(p->last_pipe, STDIN_FILENO);
	if (cmd->next)
		dup2(p->curr[1], STDOUT_FILENO);
	if (check_file_descriptors(cmd) == -1)
		exit_program(shell, 1);
	if (cmd->infile != STDIN_FILENO)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
	}
	if (cmd->outfile != STDOUT_FILENO)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
	}
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

void	execute_system_command(t_shell *shell, t_cmd *curr_cmd, t_pipe *p)
{
	if (curr_cmd->next)
		pipe(p->curr);
	p->children[p->i] = fork();
	if (p->children[p->i] == 0)
		pipe_process(shell, curr_cmd, p);
	check_for_next_pipe(p, curr_cmd);
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
	while (curr_cmd)
	{
		if (is_built_in_command(curr_cmd->args[0]) && !curr_cmd->next && p.i == 0)
		{
			if (check_file_descriptors(curr_cmd) == -1)
				shell->exit_status = 1;
			else
			{
				stdout_dup = dup(STDOUT_FILENO);
				if (curr_cmd->outfile != STDOUT_FILENO)
					dup2(curr_cmd->outfile, STDOUT_FILENO);
				shell->exit_status = execute_built_in_command(shell, curr_cmd);
				dup2(stdout_dup, STDOUT_FILENO);
				close(stdout_dup);
				if (curr_cmd->outfile != STDOUT_FILENO)
					close (curr_cmd->outfile);
			}
		}
		else
		{
			execute_system_command(shell, curr_cmd, &p);
			p.i++;
		}
		curr_cmd = curr_cmd->next;
	}
	check_for_next_pipe(&p, NULL);
	wait_children(shell, p.children);
}
