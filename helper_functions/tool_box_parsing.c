/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 17:58:55 by cocozhu           #+#    #+#             */
/*   Updated: 2026/05/26 18:00:36 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_sep(char *cur)
{
	int	i;

	i = 0;
	while (cur[i])
	{
		if (cur[i] == '=')
			return (i);
		i++;
	}
	return (0);
}

int	is_delimiter(char c)
{
	if (is_space(c) || c == '\'' || c == '\"'
		|| c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

int	is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}


char	*get_literal_chunk(char *input, int *i, int hdoc_flag)
{
	int	start;

	start = *i;
	while ((input[*i] && input[*i] != '\"')
		&& (input[*i] != '$' || hdoc_flag))
		(*i)++;
	return (ft_substr(input, start, (*i) - start));
}

char	*get_word_chunk(char *input, int *i, int hdoc_flag)
{
	int	start;

	start = *i;
	while ((input[*i] && !is_delimiter(input[*i]))
		&& (input[*i] != '$' || hdoc_flag))
		(*i)++;
	return (ft_substr(input, start, (*i) - start));
}