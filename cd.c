/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 20:51:19 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/28 23:11:08 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    swap_dir(t_shell *shell, t_env *oldpwd)
{
    // dont swap pointers. need to rewrite this function
    t_env   *curr;
    char    *tmp;

    curr = get_env_node(shell, "PWD");
    tmp = curr->value;
    curr->value = oldpwd->value;
    oldpwd->value = tmp;
}

int     change_dir(t_shell *shell, char **args)
{
    t_env    *target_node;
    
    if (args[2])
        return (ft_putstr_fd("Too many argumants\n", 2), 1);
    if (args[1] == NULL)
    {
        target_node = get_env_node(shell, "HOME");
        chdir(target_node->value);
    }
    else if (ft_strcmp(args[1], "-") == 0)
    {
        target_node = get_env_node(shell, "OLDPWD");
        if (!target_node)
            return(ft_putstr_fd("minishell: cd: OLDPWD not set", 2), 1);
        printf("%s\n", target_node->value);
        swap_dir(shell, target_node);
    }
    else
    {
        target_node = get_env_node(shell, "OLDPWD");
        if (access(args[1], X_OK) == 0)
            return 1;
        if (chdir(args[1]) == -1)
        {
            ft_putstr_fd("minishell: cd: No such file or directory", 2);
            shell->exit_status = 1;
            return (1);
        }
        swap_dir(shell, target_node);
    }
    return (0);
}