/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 15:17:23 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/05/26 18:06:37 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_hd_limit(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_HEREDOC)
			count++;
		tokens = tokens->next;
	}
	if (count > 16)
	{
		printf("minishell: maximum here-document count exceeded\n");
		return (1);
	}
	return (0);
}

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

	if (check_hd_limit(tokens) == 1)
		return (1);
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
