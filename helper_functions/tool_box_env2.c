/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_env2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:50:22 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/23 16:20:46 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_env_value(t_env *node, char *new_val)
{
	if (!node || !new_val)
		return ;
	free(node->value);
	node->value = ft_strdup(new_val);
}
