/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 21:49:28 by karmanz           #+#    #+#             */
/*   Updated: 2026/05/03 15:14:30 by zkarman          ###   ########.fr       */
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

void	close_if_non_standard_in_out_file(int infile, int outfile)
{
	if (infile)
	{
		if (infile > 0)
			close(infile);
	}
	if (outfile)
	{
		if (outfile > 1)
			close(outfile);
	}
}
