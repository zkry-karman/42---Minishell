/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_token_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 13:32:54 by cocozhu           #+#    #+#             */
/*   Updated: 2026/04/16 14:47:16 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_operator(char *input, int *i)
{
	if (input[*i] == '<')
	{
		if (input[(*i + 1)] == '<')
		{
			*i = *i + 2;
			return (ft_strdup("<<"));
		}
		(*i)++;
		return (ft_strdup("<"));
	}
	else if (input[*i] == '>')
	{
		if (input[(*i + 1)] == '>')
		{
			*i = *i + 2;
			return (ft_strdup(">>"));
		}
		(*i)++;
		return (ft_strdup(">"));
	}
	(*i)++;
	return (ft_strdup("|"));
}

t_token_type identify_type(char *value)
{
	
	if (ft_strncmp(value, "<<", 3) == 0)
		return (TOKEN_HEREDOC);
	else if (ft_strncmp(value, ">>", 3) == 0)
		return (TOKEN_APPEND);
	else if (ft_strncmp(value, "|", 2) == 0)
		return (TOKEN_PIPE);
	else if (ft_strncmp(value, "<", 2) == 0)
		return (TOKEN_IN);
	else if (ft_strncmp(value, ">", 2) == 0)
		return (TOKEN_OUT);
	return (TOKEN_WORD);
}