/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_env2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 15:50:22 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/26 18:05:20 by kzhu@student.42.f###   ########.fr       */
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

char	*check_access(char *path)
{
	if (!path)
		return (NULL);
	if (access(path, X_OK) == 0)
		return (ft_strdup(path));
	return (NULL);
}
