/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 16:45:23 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/05 17:30:22 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_execve_failure(t_shell *shell, char **env_arr, t_cmd *cmd)
{
	int	err_no;

	err_no = errno;
	free_array(envp_arr);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(err_no), 2);
	ft_putstr_fd("\n", 2);
	if (err_no == ENOENT)
		exit_program(shell, 127);
	exit_program(shell, 126);
}

void	exit_no_access(t_shell *shell, char *path, char **envp_arr)
{
	free_array(envp_arr);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	exit_program(shell, 126);
}

void	exit_no_path(t_shell *shell, char **envp_arr, t_cmd *cmd)
{
	free_array(envp_arr);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	exit_program(shell, 127);
}
