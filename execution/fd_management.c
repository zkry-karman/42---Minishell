/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_management.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 14:22:46 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/03 16:18:46 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	manage_append(t_cmd *cmd, t_redir *curr)
{
	if (cmd->outfile > 1)
		close (cmd->outfile);
	cmd->outfile = open(curr->file, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (cmd->outfile == -1)
		return (perror(curr->file), -1);
	return (0);
}

int	manage_outfile(t_cmd *cmd, t_redir *curr)
{
	if (cmd->outfile > 1)
		close (cmd->outfile);
	cmd->outfile = open(curr->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->outfile == -1)
		return (perror(curr->file), -1);
	return (0);
}

int	manage_infile(t_cmd *cmd, t_redir *curr)
{
	if (cmd->infile > 0)
		close (cmd->infile);
	cmd->infile = open(curr->file, O_RDONLY);
	if (cmd->infile == -1)
		return (perror(curr->file), -1);
	return (0);
}

int	check_file_descriptors(t_cmd *cmd)
{
	t_redir	*curr;

	curr = cmd->redirs;
	while (curr)
	{
		if (curr->type == TOKEN_IN)
		{
			if (manage_infile(cmd, curr))
				return (-1);
		}
		else if (curr->type == TOKEN_OUT)
		{
			if (manage_outfile(cmd, curr))
				return (-1);
		}
		else if (curr->type == TOKEN_APPEND)
		{
			if (manage_append(cmd, curr))
				return (-1);
		}
		curr = curr->next;
	}
	return (0);
}
