/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_token_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:33:28 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/28 20:47:04 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_d_quote(t_shell *shell, char *input, int *i)
{
	int	start;
	char *chunk;
	char *final;
	
	final = ft_strdup("");
	while (input[*i] && (input[*i] != '\"'))
	{
		if (input[*i] == '$')
			chunk = extract_env(shell, input, i);
		else
		{
			start = *i;
			while (input[*i] && input[*i] != '\"' && input[*i] != '$')
				(*i)++;
			chunk = ft_substr(input, start, (*i) - start);
		}
			final = join_and_free(final, chunk);
	}
	if (input[*i] == '\0')
			return (shell->exit_status = 2, free(final), printf("error: unclosed quote\n"), NULL);
	(*i)++;
	return (final);
}

char	*extract_quote(t_shell *shell, char *input, int *i)
{
	int	start;
	char *final;
	char quote_type;
	
	quote_type = input[*i];
	(*i)++;
	if (quote_type == '\'')
	{
		start = *i;
		while (input[*i] && (input[*i] != '\''))
			(*i)++;
		if (input[*i] == '\0')
			return (shell->exit_status = 2, printf("error: unclosed quote\n"), NULL);
		final = ft_substr(input, start, (*i) - start);
		(*i)++;
	}
	else
		final = extract_d_quote(shell, input, i);
	return (final);
}

char	*extract_word(t_shell *shell, char *input, int *i)
{
	int start;
	char *final;
	char *chunk;

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
		final = join_and_free(final, chunk);
	}
	return (final);
}

char	*extract_token(t_shell *shell, char *input, int *i)
{
	char 	*cur_token;
	char	*final_token;
	
	if (input[*i] == '<' || input[*i] == '>' || input[*i] == '|')
		return (extract_operator(input, i));
	final_token = ft_strdup("");
	while (input[*i] && is_space(input[*i]) == 0 &&
		input[*i] != '<' && input[*i] != '>' && input[*i] != '|')
	{
		if (input[*i] == '\'' || input[*i] == '\"')
			cur_token = extract_quote(shell, input, i);
		else
			cur_token = extract_word(shell, input, i);
		if (cur_token == NULL)
			return (free(final_token), NULL);
		final_token = join_and_free(final_token, cur_token);
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
			return (free_tokens(&(shell->input_list)), 1);
		append_node(&(shell->input_list), cur_token);
	}
	return (0);
}