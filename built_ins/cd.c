/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 20:51:19 by karmanz           #+#    #+#             */
/*   Updated: 2026/05/01 14:43:53 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     go_to_new_dir(t_shell *shell, char *new_path)
{
    char    cwd[1024];
    t_env   *target_node;
    
    if (!getcwd(cwd, 1024))
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
    if (getcwd(cwd, 1024))
    {
        target_node = get_env_node(shell, "PWD");
        if (target_node)
            replace_env_value(target_node, cwd);
    }
    return (0);
}

char     *go_to_last_working_dir(t_shell *shell)
{
    t_env   *target_node;
    
    target_node = get_env_node(shell, "OLDPWD");
    if (!target_node)
        return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), NULL);
    printf("%s\n", target_node->value);
    return (target_node->value);
}

char    *go_to_home_dir(t_shell *shell)
{
    t_env   *target_node;
    
    target_node = get_env_node(shell, "HOME");
    if (!target_node)
        return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), NULL);
    return (target_node->value);
}

int     change_dir(t_shell *shell, char **args)
{
    char    *new_path;
    
    if (args[1] && args[2])
        return (ft_putstr_fd("Too many argumants\n", 2), 1);
    if (args[1] == NULL)
        new_path = go_to_home_dir(shell);
    else if (ft_strcmp(args[1], "-") == 0)
        new_path = go_to_last_working_dir(shell, target_node, new_path);
    else
        new_path = args[1];
    if (!path)
        return (1);
    return (go_to_new_dir(shell, new_path));
}
