/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:45:23 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/15 18:26:14 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	kill_child(t_shell *shell, int exit_code)
{
	if (shell)
	{
		if (shell->cmds)
			free_cmds(&(shell->cmds));
		if (shell->env_list)
			free_env(&(shell->env_list));
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	exit(exit_code);
}

void	exit_execve_failure(t_shell *shell, char **env_arr, t_cmd *cmd)
{
	int	err_no;

	err_no = errno;
	free_array(env_arr);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(err_no), 2);
	ft_putstr_fd("\n", 2);
	if (err_no == ENOENT)
		kill_child(shell, 127);
	kill_child(shell, 126);
}

void	exit_no_access(t_shell *shell, char *path, char **envp_arr)
{
	free_array(envp_arr);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	kill_child(shell, 126);
}

void	exit_no_path(t_shell *shell, char **envp_arr, t_cmd *cmd)
{
	free_array(envp_arr);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	kill_child(shell, 127);
}
