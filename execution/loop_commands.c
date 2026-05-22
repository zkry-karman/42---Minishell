/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:14:40 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/22 18:10:24 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_built_in_command(t_shell *shell, t_cmd *cmd, int *stout_dup)
{
	int		target_out;
	int		target_in;;

	(void)stout_dup;
	shell->backup_stdin = dup(STDIN_FILENO);
	shell->backup_stdout = dup(STDOUT_FILENO);
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
	dup2(shell->backup_stdin, STDIN_FILENO);
	dup2(shell->backup_stdout, STDOUT_FILENO);
	check_backups(shell);
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
