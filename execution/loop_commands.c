/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:14:40 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/23 13:10:01 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_built_in_command(t_shell *shell, t_cmd *cmd, int *stout_dup)
{
	int		target_out;
	int		target_in;

	(void)stout_dup;
	target_out = cmd->outfile;
	target_in = cmd->infile;
	if (target_out > 2)
	{
		dup2(target_out, STDOUT_FILENO);
		close(target_out);
		cmd->outfile = -1;
	}
	if (target_in > 2)
	{
		dup2(target_in, STDIN_FILENO);
		close(target_in);
		cmd->infile = -1;
	}
	shell->exit_status = execute_built_in_command(shell, cmd);
	dup2(shell->backup_stdout, STDOUT_FILENO);
	dup2(shell->backup_stdin, STDIN_FILENO);
	close_if_non_standard_in_out_file(&cmd->infile, &cmd->outfile);
}

int	loop_cmds(t_shell *shell, t_pipe *p, t_cmd *cmd, int stout_dup)
{
	stout_dup = -1;
	while (cmd)
	{
		if (is_built_in_command(cmd->args[0]) && !cmd->next && p->i == 0)
		{
			if (check_file_descriptors(cmd) == -1)
				shell->exit_status = 1;
			else
				setup_built_in_command(shell, cmd, &stout_dup);
		}
		else
		{
			if (execute_system_command(shell, cmd, p) != 0)
				return (1);
			p->i++;
		}
		cmd = cmd->next;
	}
	return (0);
}
