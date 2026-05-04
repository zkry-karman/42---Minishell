/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/04 17:35:11 by karmanz          ###   ########.fr       */
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

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	envp_arr = envp_list_to_arr(shell);
	path = get_path(cmd->args[0], envp_arr);
	if (!path)
	{
		free_array(envp_arr);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit_program(shell, 127);
	}
	if (execve(path, cmd->args, envp_arr) == -1)
	{
		free_array(envp_arr);
		perror("Execve Failure");
		exit_program(shell, 1);
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
		if (is_built_in_command(curr_cmd->args[0]))
			shell->exit_status = execute_built_in_command(shell, curr_cmd);
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
