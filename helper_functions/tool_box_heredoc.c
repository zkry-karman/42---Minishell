/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 18:41:28 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/17 18:59:18 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_hd(t_shell *shell, t_redir *curr, char *line, int fd[2])
{
	while (1)
	{
		line = readline("> ");
		if (g_status == 130
			|| ft_strncmp(line, curr->file, ft_strlen(curr->file) + 1) == 0)
			break ;
		if (curr->quoted == 0)
			line = expand_heredoc(shell, line);
		ft_putstr_fd(line, fd[1]);
		ft_putstr_fd("\n", fd[1]);
		free(line);
	}
}

int	setup_heredoc(t_shell *shell, t_redir *redir, t_cmd *cmd)
{
	if (cmd->infile > 0)
		close(cmd->infile);
	cmd->infile = handle_heredoc(shell, redir);
	if (cmd->infile == -1)
		return (1);
	if (shell->exit_status == 130)
		return (0);
	return (0);
}
