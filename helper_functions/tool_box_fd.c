/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 21:49:28 by karmanz           #+#    #+#             */
/*   Updated: 2026/05/15 17:55:14 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_for_next_pipe(t_pipe *p, t_cmd *curr_cmd)
{
	if (p->last_pipe != -1)
		close(p->last_pipe);
	if (curr_cmd)
	{
		if (curr_cmd->next)
		{
			close(p->curr[1]);
			p->last_pipe = p->curr[0];
		}
		else
			p->last_pipe = -1;
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
