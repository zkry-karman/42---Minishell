/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_managment.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:19:08 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/24 19:24:41 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    handle_heredoc(char *limiter)
{
    int     fd[2];
    char    *line;

    if (pipe(fd) == -1)
    {
        perror("minishell: pipe");
        return (-1);
    }
    while (1)
    {
        line = readline("> ");
        if (!line)
            break ;
        if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
        {
            free(line);
            break ;
        }
        ft_putstr_fd(line, fd[1]);
        ft_putstr_fd("\n", fd[1]);
        free(line);
    }
    close(fd[1]);
    return(fd[0]);
}

void    check_heredocs(t_shell *shell)
{
    t_cmd   *curr_cmd;
    t_redir *curr_redir;

    curr_cmd = shell->cmds;
    while (curr_cmd)
    {
        curr_redir = curr_cmd->redirs;
        while (curr_redir)
        {
            if (curr_redir->type == TOKEN_HEREDOC)
            {
                if (curr_cmd->infile > 0)
                    close(curr_cmd->infile);
                curr_cmd->infile = handle_heredoc(curr_redir->file);
            }
            curr_redir = curr_redir->next;
        }
        curr_cmd = curr_cmd->next;
    }
}