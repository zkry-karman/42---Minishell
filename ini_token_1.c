/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_token_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:33:28 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/16 14:59:10 by cocozhu          ###   ########.fr       */
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

char	*extract_quote(char *input, int *i)
{
	int	start;
	char quote_type;
	char *token;
	
	quote_type = input[*i];
	start = ++(*i);
	while (input[*i] && (input[*i] != quote_type))
		(*i)++;
	if (input[*i] == '\0')
		return (printf("error: unclosed quote\n"), NULL);
	token = ft_substr(input, start, (*i) - start);
	(*i)++;
	return (token);
}

char	*extract_word(char *input, int *i)
{
	int start;
	
	start = *i;
	while (is_space(input[*i]) == 0 && input[*i]
			&& input[*i] != '\'' && input[*i] != '\"'
			&& input[*i] != '<' && input[*i] != '>' && input[*i] != '|')
		(*i)++;
	return (ft_substr(input, start, (*i) - start));
}

char	*extract_token(char *input, int *i)
{
	char 	*cur_token;
	char	*final_token;
	char	*temp;
	
	final_token = ft_strdup("");
	if (input[*i] == '<' || input[*i] == '>' || input[*i] == '|')
		return (extract_operator(input, i));
	while (input[*i] && is_space(input[*i]) == 0 &&
		input[*i] != '<' && input[*i] != '>' && input[*i] != '|')
	{
		if (input[*i] == '\'' || input[*i] == '\"')
			cur_token = extract_quote(input, i);
		else
			cur_token = extract_word(input, i);
		if (cur_token == NULL)
			return (free(final_token), NULL);
		temp = ft_strjoin(final_token, cur_token);
		free(final_token);
		free(cur_token);
		final_token = temp;
	}
	return (final_token);
}

int	build_token(t_token **input_list, char *input)
{
	int		i;
	char	*cur_token;

	i = 0;
	while (input[i])
	{
		while (is_space(input[i]) == 1)
			i++;
		if (input[i] == '\0')
			break ; 
		cur_token = extract_token(input, &i);
		if (cur_token == NULL)
			return (1);
		append_node(input_list, cur_token);
	}
	return (0);
}
