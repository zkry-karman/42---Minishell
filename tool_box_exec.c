/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 11:11:06 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/29 11:11:27 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     command_count(t_cmd *cmds)
{
    int     i;
    t_cmd   *tmp;
    
    i = 0;
    tmp = cmds;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}