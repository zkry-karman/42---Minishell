/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 15:17:23 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/05/02 11:18:44 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	syntax_error_printer(int i, char *value)
{
	if (i == 1)
		printf("minishell: syntax error near unexpected token `|'\n");
	if (i == 2)
		printf ("minishell: syntax error near unexpected token `newline'\n");
	if (i == 3)
		printf ("minishell: syntax error near unexpected token `%s'\n", value);
}

int	syntax_checker(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	if (cur && cur->type == TOKEN_PIPE)
		return (syntax_error_printer(1, NULL), 1);
	while (cur)
	{
		if (cur->type > TOKEN_PIPE
			&& (cur->next == NULL || cur->next->type != TOKEN_WORD))
		{
			if (cur->next == NULL)
				return (syntax_error_printer(2, NULL), 1);
			return (syntax_error_printer(3, cur->next->value), 1);
		}
		if (cur->type == TOKEN_PIPE
			&& (cur->next == NULL || cur->next->type == TOKEN_PIPE))
			return (syntax_error_printer(1, NULL), 1);
		cur = cur->next;
	}
	return (0);
}
