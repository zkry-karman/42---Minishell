/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 15:00:28 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/29 15:14:46 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    remove_env(t_shell *shell, char *key)
{
    t_env   *curr;
    t_env   *prev;

    curr = shell->env_list;
    prev = NULL;
    while (curr && ft_strcmp(curr->key, key) != 0)
    {
        prev = curr;
        curr = curr->next;
    }
    if (curr)
    {
        if (prev == NULL)
            shell->env_list = curr->next;
        else
            prev->next = curr->next;
        free(curr->key);
        free(curr->value);
        free(curr);
    }
}

int     ft_unset(t_shell *shell, char **args)
{
    int     i;

    i = 1;
    while (args[i])
    {
        remove_env(shell, args[i]);
        i++;
    }
    return (0);
}