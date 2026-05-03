/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:19:08 by karmanz           #+#    #+#             */
/*   Updated: 2026/05/03 16:19:15 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_error_msg(t_redir *curr)
{
	printf("minishell: warning: here-document");
	printf(" delimited by end-of-file (wanted '%s')\n", curr->file);
}

char	*expand_heredoc(t_shell *shell, char *line)
{
	int		i;
	int		start;
	char	*chunk;
	char	*final;

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

int	handle_heredoc(t_shell *shell, t_redir *curr)
{
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (perror("minishell: pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			heredoc_error_msg(curr);
			break ;
		}
		if (ft_strncmp(line, curr->file, ft_strlen(curr->file) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (curr->quoted == 0)
			line = expand_heredoc(shell, line);
		ft_putstr_fd(line, fd[1]);
		ft_putstr_fd("\n", fd[1]);
		free(line);
	}
	return (close(fd[1]), fd[0]);
}

void	check_heredocs(t_shell *shell)
{
	t_cmd	*curr_cmd;
	t_redir	*curr_redir;

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
				curr_cmd->infile = handle_heredoc(shell, curr_redir);
			}
			curr_redir = curr_redir->next;
		}
		curr_cmd = curr_cmd->next;
	}
}
