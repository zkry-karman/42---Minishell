/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_management.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 14:22:46 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/23 15:12:02 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    check_file_descriptors(t_cmd *cmd)
{
    t_redir   *curr;

    curr = cmd->redirs;
    while (curr)
    {
        if (curr->type == TOKEN_OUT)
        {
            if (cmd->infile > 0)
                close (cmd->infile);
            cmd->infile = open(curr->file, O_RDONLY);
            if (cmd->infile == -1)
                return (perror(curr->file), -1);
        }
        else if (curr->type == TOKEN_OUT)
        {
            if (cmd->outfile > 1)
                close (cmd->outfile);
            cmd->outfile = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (cmd->outfile == -1)
                return (perro(curr->file), -1);
        }
        else if (curr->type == TOKEN_APPEND)
        {
            if (cmd->outfile > 1)
                close(cmd->outfile);
            cmd->outfile = open(curr->file, O_WRONLY | O_CREAT | O_APPEND, 0664);
            if (cmd->outfile == -1)
                return (perror(curr->file), -1);
        }
        curr = curr->next;
    }
    return (0);
}