/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 21:49:28 by karmanz           #+#    #+#             */
/*   Updated: 2026/05/23 15:42:27 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_next_pipe(t_pipe *p, int *old_pipe)
{
	if (p->curr[1] > 2)
	{
		close(p->curr[1]);
		p->curr[1] = -1;
	}
	*old_pipe = p->last_pipe;
	p->last_pipe = p->curr[0];
	p->curr[0] = -1;
	if (*old_pipe != -1)
		close(*old_pipe);
}

void	check_for_next_pipe(t_pipe *p, t_cmd *curr_cmd)
{
	int		old_pipe;

	if (curr_cmd)
	{
		if (curr_cmd->next)
			setup_next_pipe(p, &old_pipe);
		else
		{
			if (p->last_pipe != -1)
			{
				close(p->last_pipe);
				p->last_pipe = -1;
			}
			if (p->curr[1] > 2)
			{
				close(p->curr[1]);
				p->curr[1] = -1;
			}
			if (p->curr[0] > 2)
			{
				close(p->curr[0]);
				p->curr[0] = -1;
			}
		}
	}
}

void	close_if_non_standard_in_out_file(int *infile, int *outfile)
{
	if (infile && *infile > 2)
	{
		close(*infile);
		*infile = -1;
	}
	if (outfile && *outfile > 2)
	{
		close(*outfile);
		*outfile = -1;
	}
}

void	verify_stds(t_cmd *cmd)
{
	if (cmd->infile != STDIN_FILENO && cmd->infile > 0)
	{
		dup2(cmd->infile, STDIN_FILENO);
		close(cmd->infile);
		cmd->infile = STDIN_FILENO;
	}
	if (cmd->outfile != STDOUT_FILENO && cmd->outfile > 1)
	{
		dup2(cmd->outfile, STDOUT_FILENO);
		close(cmd->outfile);
		cmd->outfile = STDOUT_FILENO;
	}
}

void	check_backups(t_shell *shell)
{
	if (shell->backup_stdin != -1)
	{
		close(shell->backup_stdin);
		shell->backup_stdin = -1;
	}
	if (shell->backup_stdout != -1)
	{
		close(shell->backup_stdout);
		shell->backup_stdout = -1;
	}
}
