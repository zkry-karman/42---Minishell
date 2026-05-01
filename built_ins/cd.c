/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 20:51:19 by karmanz           #+#    #+#             */
/*   Updated: 2026/05/01 12:50:28 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     go_to_new_dir(t_shell *shell, t_env *target_node, char *new_path, char cwd[1024])
{
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

int     go_to_last_working_dir(t_shell *shell, t_env *target_node, char *new_path)
{
    target_node = get_env_node(shell, "OLDPWD");
    if (!target_node)
        return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), 1);
    new_path = target_node->value;
    printf("%s\n", new_path);
    return (0);
}

int    go_to_home_dir(t_shell *shell, t_env *target_node, char *new_path)
{
    target_node = get_env_node(shell, "HOME");
    if (!target_node)
        return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
    new_path = target_node->value;
    printf("%s\n", new_path);
    return (0);
}

int     change_dir(t_shell *shell, char **args)
{
    t_env    *target_node;
    char    *new_path;
    char    cwd[1024];
    
    if (args[1] && args[2])
        return (ft_putstr_fd("Too many argumants\n", 2), 1);
    if (args[1] == NULL)
    {
        if(go_to_home_dir(shell, target_node, new_path))
            return (1);
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        if(go_to_last_working_dir(shell, target_node, new_path))
            return (1);
    }
    else
    {
        new_path = args[1];
        if (go_to_new_dir(shell, target_node, cwd))
            return (1);
    }
    return (0);
}
