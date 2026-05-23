/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_env2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:50:22 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/23 15:58:10 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char    *link_and_join(t_env *node)
{
    char    *temp;
    char    *value_to_join;
    char    *result;

    temp = ft_strjoin(node->key, "=");
    if (!temp)
        return (NULL);
    if (node->value)
        value_to_join = node->key
}