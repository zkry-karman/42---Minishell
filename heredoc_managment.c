/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_managment.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:19:08 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/22 15:32:01 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    handle_heredoc(char *limiter)
{
    
}

void    check_heredocs(t_shell *shell)
{
    t_cmd   *curr_cmd;

    curr_cmd = shell->cmds;
    
    while (curr_cmd)
    {
        if (curr_cmd->limiter)
        {
            if (curr_cmd->infile != 0)
                close(curr_cmd->infile);
            curr_cmd->infile = handle_heredoc(curr_cmd->limiter);
        }
        curr_cmd = curr_cmd->next;
    }
}