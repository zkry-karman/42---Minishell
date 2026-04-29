/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 20:15:06 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/29 15:38:06 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    execute_built_in_command(t_shell *shell, t_cmd *cmd)
{
    if (ft_strcmp(cmd->args[0], "cd") == 0)
        return (change_dir(shell, cmd->args));
    if (ft_strcmp(cmd->args[0], "echo") == 0)
        return (echo(cmd->args));
    if (ft_strcmp(cmd->args[0], "env") == 0)
        return (show_env(shell->env_list));
    if (ft_strcmp(cmd->args[0], "exit") == 0)
        return (check_exit(shell, cmd->args));
    if (ft_strcmp(cmd->args[0], "export") == 0)
        return (ft_export(shell, cmd->args));
    if (ft_strcmp(cmd->args[0], "pwd") == 0)
        return (print_cwd(shell, cmd->args));
    if (ft_strcmp(cmd->args[0], "unset") == 0)
        return (ft_unset(shell, cmd->args));
    return (0);
}

int     is_built_in_command(char *cmd)
{
    if (!cmd)
        return (0);
    if (ft_strcmp(cmd, "cd") == 0
            || ft_strcmp(cmd, "echo") == 0
            || ft_strcmp(cmd, "env") == 0
            || ft_strcmp(cmd, "exit") == 0
            || ft_strcmp(cmd, "export") == 0
            || ft_strcmp(cmd, "pwd") == 0
            || ft_strcmp(cmd, "unset") == 0)
        return (1);
    return (0);
}