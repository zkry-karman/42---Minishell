/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 12:14:49 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/29 12:22:08 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     show_env(t_env *env_list)
{
    t_env   *tmp;

    if (!env_list)
        return (0);
    tmp = env_list;
    while (tmp)
    {
        ft_putstr_fd(tmp->key, 1);
        ft_putstr_fd("=", 1);
        ft_putendl_fd(tmp->value, 1);
        tmp = tmp->next;
    }
    return (0);
}