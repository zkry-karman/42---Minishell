/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:14:40 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/05 17:32:41 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_commands(t_shell *shell, t_pipe *p, t_cmd *cmd, int stout_dup)
{
	while (cmd)
	{
		if (is_built_in_command(cmd->args[0]) && !cmd->next && p->i == 0)
		{
			if (check_file_descriptors(cmd) == -1)
				shell->exit_status = 1;
			else
			{
				stout_dup = dup(STDOUT_FILENO);
				if (cmd->outfile != STDOUT_FILENO)
					dup2(cmd->outfile, STDOUT_FILENO);
				shell->exit_status = execute_built_in_command(shell, cmd);
				dup2(stout_dup, STDOUT_FILENO);
				close(stout_dup);
				if (cmd->outfile != STDOUT_FILENO)
					close (cmd->outfile);
			}
		}
		else
		{
			execute_system_command(shell, cmd, p);
			p->i++;
		}
		cmd = cmd->next;
	}
}
