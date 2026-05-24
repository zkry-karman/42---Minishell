/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/24 14:43:48 by cocozhu           #+#    #+#             */
/*   Updated: 2026/05/24 15:26:35 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*hide_spaces(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == ' ')
			str[i] = '\1';
		i++;
	}
	return (str);
}

char	*restore_spaces(char *str)
{
	int i;
	
	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\1')
			str[i] = ' ';
		i++;
	}
	return (str);
}