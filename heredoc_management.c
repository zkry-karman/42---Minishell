/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:19:08 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/29 15:50:03 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *expand_heredoc(t_shell *shell, char *line)
{
    int i;
    int	start;
	char *chunk;
	char *final;
	
    i = 0;
	final = ft_strdup("");
	while (line[i])
	{
		if (line[i] == '$')
			chunk = extract_env(shell, line, &i);
		else
		{
			start = i;
			while (line[i] && line[i] != '$')
				i++;
			chunk = ft_substr(line, start, i - start);
		}
			final = join_and_free(final, chunk);
	}
	free(line);
	return (final);
}

int    handle_heredoc(t_shell *shell, char *limiter, int quoted)
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
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", limiter);
            break ;
        }
        if (ft_strncmp(line, limiter, ft_strlen(limiter) + 1) == 0)
        {
            free(line);
            break ;
        }
		if (quoted == 0)
        	line = expand_heredoc(shell, line);
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
                curr_cmd->infile = handle_heredoc(shell, curr_redir->file, curr_redir->quoted);
            }
            curr_redir = curr_redir->next;
        }
        curr_cmd = curr_cmd->next;
    }
}