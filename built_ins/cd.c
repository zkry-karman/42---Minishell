/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 20:51:19 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/29 15:20:45 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     change_dir(t_shell *shell, char **args)
{
    t_env    *target_node;
    char    *new_path;
    char    cwd[1024];
    
    
    if (args[1] && args[2])
        return (ft_putstr_fd("Too many argumants\n", 2), 1);
    if (args[1] == NULL)
    {
        target_node = get_env_node(shell, "HOME");
        if (!target_node)
            return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
        new_path = target_node->value;
        printf("%s\n", new_path);
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        target_node = get_env_node(shell, "OLDPWD");
        if (!target_node)
            return(ft_putstr_fd("minishell: cd: OLDPWD not set", 2), 1);
        new_path = target_node->value;
        printf("%s\n", new_path);
    }
    else
        new_path = args[1];
    if (!getcwd(cwd, sizeof(cwd)))
        return (1);
    if (chdir(new_path) == -1)
    {
        perror("minishell: cd");
        shell->exit_status = 1;
        return (1);
    }
    target_node = get_env_node(shell, "OLDPWD");
    if (target_node)
        replace_env_value(target_node, cwd);
    if (getcwd(cwd, sizeof(cwd)))
    {
        target_node = get_env_node(shell, "PWD");
        if (target_node)
            replace_env_value(target_node, cwd);
    }
    shell->exit_status = 0;
    return (0);
}
