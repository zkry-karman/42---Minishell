/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_children.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 14:05:27 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/23 15:00:46 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_signal_interuptions(t_shell *shell, int status)
{
	if (WTERMSIG(status) == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		shell->exit_status = 131;
	}
	else if (WTERMSIG(status) == SIGINT)
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		shell->exit_status = 130;
	}
}

void	wait_children(t_shell *shell, pid_t *child, int child_count)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < child_count)
	{
		if (child[i] >= 0)
		{
			waitpid(child[i], &status, 0);
			if (i == child_count - 1)
			{
				if (WIFEXITED(status))
					shell->exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					check_signal_interuptions(shell, status);
			}
		}
		i++;
	}
}

void	initialize_children(t_shell *shell, t_pipe *p)
{
	int		i;

	p->children = malloc(sizeof(pid_t) * command_count(shell->cmds));
	if (!p->children)
		return ;
	i = 0;
	while (i < command_count(shell->cmds))
		p->children[i++] = -1;
	return ;
}

void    clean_up_children(t_shell *shell, t_pipe *p)
{
    wait_children(shell, p->children, command_count(shell->cmds));
    free(p->children);
    shell->pipe_processes = NULL;
    setup_prompt_signals();
}

void    children_failure(t_shell *shell)
{
    shell->pipe_processes = NULL;
    setup_prompt_signals();
    return ;
}