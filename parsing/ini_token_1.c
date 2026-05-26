/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_token_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:33:28 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/05/26 18:07:29 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_d_quote(t_shell *shell, char *input, int *i)
{
	char	*chunk;
	char	*final;
	int		hdoc_flag;

	final = ft_strdup("");
	if (!final)
		return (NULL);
	hdoc_flag = is_hdoc(shell->input_list);
	while (input[*i] && (input[*i] != '\"'))
	{
		if (input[*i] == '$' && !hdoc_flag)
			chunk = extract_env(shell, input, i);
		else
			chunk = get_literal_chunk(input, i, hdoc_flag);
		final = join_and_free(final, chunk);
		if (!final)
			return (NULL);
	}
	if (input[*i] == '\0')
		return (shell->exit_status = 2,
			free(final), printf("error: unclosed quote\n"), NULL);
	return ((*i)++, hide_spaces(final));
}

char	*extract_quote(t_shell *shell, char *input, int *i)
{
	int		start;
	char	*final;
	char	quote_type;

	quote_type = input[*i];
	(*i)++;
	if (quote_type == '\'')
	{
		start = *i;
		while (input[*i] && (input[*i] != '\''))
			(*i)++;
		if (input[*i] == '\0')
			return (shell->exit_status = 2,
				printf("error: unclosed quote\n"), NULL);
		final = ft_substr(input, start, (*i) - start);
		(*i)++;
		return (hide_spaces(final));
	}
	else
		final = extract_d_quote(shell, input, i);
	return (final);
}

char	*extract_word(t_shell *shell, char *input, int *i)
{
	char	*final;
	char	*chunk;
	int		hdoc_flag;

	final = ft_strdup("");
	if (!final)
		return (NULL);
	hdoc_flag = is_hdoc(shell->input_list);
	while (input[*i] && !is_delimiter(input[*i]))
	{
		if (g_status == 130)
			return (free(final), NULL);
		if (input[*i] == '$' && !hdoc_flag)
			chunk = extract_env(shell, input, i);
		else
			chunk = get_word_chunk(input, i, hdoc_flag);
		final = join_and_free(final, chunk);
		if (!final)
			return (NULL);
	}
	return (final);
}

char	*extract_token(t_shell *shell, char *input, int *i, int *qted)
{
	char	*cur_token;
	char	*final_token;

	if (input[*i] == '<' || input[*i] == '>' || input[*i] == '|')
		return (extract_operator(input, i));
	final_token = ft_strdup("");
	while (input[*i] && is_space(input[*i]) == 0
		&& input[*i] != '<' && input[*i] != '>' && input[*i] != '|')
	{
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			*qted = 1;
			cur_token = extract_quote(shell, input, i);
		}
		else
			cur_token = extract_word(shell, input, i);
		if (!cur_token)
			return (free(final_token), NULL);
		final_token = join_and_free(final_token, cur_token);
		if (!final_token)
			return (NULL);
	}
	return (final_token);
}

int	build_token(t_shell *shell, char *input)
{
	int		i;
	int		quoted;
	char	*cur_token;

	i = 0;
	while (input[i])
	{
		while (is_space(input[i]) == 1)
			i++;
		if (input[i] == '\0')
			break ;
		quoted = 0;
		cur_token = extract_token(shell, input, &i, &quoted);
		if (cur_token == NULL)
			return (free_tokens(&(shell->input_list)), 1);
		if (cur_token[0] == '\0' && quoted == 0)
		{
			free(cur_token);
			continue ;
		}
		if (append_node(&(shell->input_list), cur_token, quoted) == 1)
			return (free(cur_token), free_tokens(&(shell->input_list)), 1);
	}
	return (0);
}
