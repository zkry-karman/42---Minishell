/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_token_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 13:32:54 by cocozhu           #+#    #+#             */
/*   Updated: 2026/04/23 18:10:25 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_node(t_token **input_list, char *token)
{
	t_token	*new_node;
	t_token	*last_node;

	new_node = malloc(sizeof(t_token));
	if (new_node == NULL)
		return (1);
	ft_memset(new_node, 0, sizeof(t_token));
	new_node->value = token;
	new_node->type = identify_type(new_node->value);
	if (*input_list == NULL)
	{
		*input_list = new_node;
		return (0);
	}
	last_node = *input_list;
	while (last_node->next != NULL)
		last_node = last_node->next;
	last_node->next = new_node;
	return (0);
}

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