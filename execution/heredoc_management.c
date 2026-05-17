/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 15:19:08 by karmanz           #+#    #+#             */
/*   Updated: 2026/05/17 18:07:26 by kzhu@student.42.f###   ########.fr       */
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
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		return (perror("minishell: pipe"), -1);
	shell->backup_stdin = dup(STDIN_FILENO);
	signal(SIGINT, heredoc_sigint);
	while (1)
	{
		line = readline("> ");
		if (g_status == 130
			|| ft_strncmp(line, curr->file, ft_strlen(curr->file) + 1) == 0)
			break;
		if (curr->quoted == 0)
			line = expand_heredoc(shell, line);
		ft_putstr_fd(line, fd[1]);
		ft_putstr_fd("\n", fd[1]);
		free(line);
	}
	if (line)
		free(line);
	else if (g_status != 130)
		heredoc_error_msg(curr);
	dup2(shell->backup_stdin, STDIN_FILENO);
	close(shell->backup_stdin);
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
			{
				if (curr_cmd->infile > 0)
					close(curr_cmd->infile);
				curr_cmd->infile = handle_heredoc(shell, curr_redir);
				if (curr_cmd->infile == -1)
					return (1);
				if (shell->exit_status == 130)
					return (0);
			}
			curr_redir = curr_redir->next;
		}
		if (shell->exit_status == 130)
			return (0);
		curr_cmd = curr_cmd->next;
	}
	return (0);
}
