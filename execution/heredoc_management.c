/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:19:08 by karmanz           #+#    #+#             */
/*   Updated: 2026/05/23 12:36:33 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int sig)
{
	(void)sig;
	g_status = 130;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

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
	int	fd[2];
	int	local_stdin_dup;

	if (pipe(fd) == -1)
		return (perror("minishell: pipe"), -1);
	local_stdin_dup = dup(STDIN_FILENO);
	signal(SIGINT, heredoc_sigint);
	read_hd(shell, curr, fd);
	if (g_status != 130)
		heredoc_error_msg(curr);
	dup2(local_stdin_dup, STDIN_FILENO);
	close(local_stdin_dup);
	signal(SIGINT, handle_sigint);
	if (g_status == 130)
		return (close(fd[0]), close(fd[1]), -1);
	return (close(fd[1]), fd[0]);
}

int	check_heredocs(t_shell *shell)
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
				if (setup_heredoc(shell, curr_redir, curr_cmd) != 0)
					return (1);
			curr_redir = curr_redir->next;
		}
		if (g_status == 130)
		{
			shell->exit_status = 130;
			return (1);
		}
		curr_cmd = curr_cmd->next;
	}
	return (0);
}
