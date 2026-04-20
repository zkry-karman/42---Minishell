/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_token_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:33:28 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/20 17:36:30 by kzhu@student.42.f###   ########.fr       */
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

char	*extract_quote(t_shell *shell, char *input, int *i)
{
	int	start;
	char quote_type;
	char *token;
	
	quote_type = input[*i];
	start = ++(*i);
	if (quote_type == '\'')
	{
		while (input[*i] && (input[*i] != quote_type))
		(*i)++;
		if (input[*i] == '\0')
		return (printf("error: unclosed quote\n"), NULL);
		token = ft_substr(input, start, (*i) - start);
		(*i)++;
	}
	else
	{
		while (input[*i] && (input[*i] != quote_type) && input[*i] != '$')
			(*i)++;
	}
	return (token);
}

char	*extract_word(t_shell *shell, char *input, int *i)
{
	int start;
	char *final;
	char *chunk;
	char *temp;

	final = ft_strdup("");
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (input[*i] == '$')
			chunk = extract_env(shell, input, i);
		else
		{
			start = *i;
			while (input[*i] != '$' && input[*i] &&
				!is_delimiter(input[*i]))
				(*i)++;
			chunk = ft_substr(input, start, (*i) - start);
		}
		temp = ft_strjoin(final, chunk);
		free (chunk);
		free (final);
		final = temp;
	}
	return (final);
}

char	*extract_token(t_shell *shell, char *input, int *i)
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
			cur_token = extract_quote(shell, input, i);
		else
			cur_token = extract_word(shell, input, i);
		if (cur_token == NULL)
			return (free(final_token), NULL);
		temp = ft_strjoin(final_token, cur_token);
		free(final_token);
		free(cur_token);
		final_token = temp;
	}
	return (final_token);
}

int	build_token(t_shell *shell, char *input)
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
		cur_token = extract_token(shell, input, &i);
		if (cur_token == NULL)
			return (1);
		append_node(&(shell->input_list), cur_token);
	}
	return (0);
}