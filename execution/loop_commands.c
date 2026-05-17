/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:14:40 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/17 16:35:18 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_built_in_command(t_shell *shell, t_cmd *cmd, int *stout_dup)
{
	int		stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	*stout_dup = dup(STDOUT_FILENO);
	shell->backup_stdin = stdin_backup;
	shell->backup_stdout = *stout_dup;
	if (cmd->outfile > 2)
		dup2(cmd->outfile, STDOUT_FILENO);
	if (cmd->infile > 2)
		dup2(cmd->infile, STDIN_FILENO);
	shell->exit_status = execute_built_in_command(shell, cmd);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(*stout_dup, STDOUT_FILENO);
	close(stdin_backup);
	close(*stout_dup);
	shell->backup_stdout = -1;
	if (cmd->outfile > 2)
	{
		close(cmd->outfile);
		cmd->outfile = STDOUT_FILENO;
	}
	if (cmd->infile > 2)
	{
		close(cmd->infile);
		cmd->infile = STDIN_FILENO;
	}
	*stout_dup = -1;
}

void	loop_cmds(t_shell *shell, t_pipe *p, t_cmd *cmd, int stout_dup)
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
			execute_system_command(shell, cmd, p);
			p->i++;
		}
		cmd = cmd->next;
	}
}
