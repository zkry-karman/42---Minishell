/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 21:49:28 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/30 22:39:19 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    check_for_next_pipe(int last_pipe, t_cmd *curr_cmd, int curr_pipe[2])
{
    if (last_pipe != -1)
        close(last_pipe);
    if (curr_cmd)
    {
        if (curr_cmd->next)
        {
            close(curr_pipe[1]);
            last_pipe = curr_pipe[0];
        }
        else
            last_pipe = -1;
    }
}

void    close_if_non_standard_in_out_file(int infile, int outfile)
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