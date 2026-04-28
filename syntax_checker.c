/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 15:17:23 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/28 16:50:01 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_checker(t_token *tokens)
{
	t_token *cur;

	if (tokens == NULL)
		return (0);
	cur = tokens;
	if (cur->type == TOKEN_PIPE)	
		return (printf("minishell: syntax error near unexpected token `|'\n"), 1);
	while (cur)
	{
		if (cur->type > TOKEN_PIPE && (cur->next == NULL || cur->next->type != TOKEN_WORD))
		{
			if (cur->next == NULL)
				return (printf ("minishell: syntax error near unexpected token `newline'\n"), 1);
			return (printf ("minishell: syntax error near unexpected token `%s'\n", cur->next->value), 1);
		}
		if (cur->type == TOKEN_PIPE && (cur->next == NULL || cur->next->type == TOKEN_PIPE))
				return (printf ("minishell: syntax error near unexpected token `|'\n"), 1);
		cur = cur->next;
	}
	return (0);
}
