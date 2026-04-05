/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:33:28 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/05 18:00:01 by cocozhu          ###   ########.fr       */
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

char	*extract_token(char *input, int *i)
{
	int start;
	char quote_type;
	char *cur_token;
	
	cur_token = NULL;
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		quote_type = input[*i];
		start = ++(*i);
		while (input[*i] && (input[*i] != quote_type))
			(*i)++;
		if (input[*i] == '\0')
			return (printf("error: unclosed quote\n"), NULL);
		cur_token = ft_substr(input, start, (*i) - start);
		(*i)++;
	}
	else if (is_space(input[*i]) == 0)
	{
		start = *i;
		while (!is_space(input[*i]) && input[*i] && input[*i] != '\'' && input[*i] != '\"' )
			(*i)++;
		cur_token = ft_substr(input, start, (*i) - start);
	}
	return (cur_token);
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

int	main()
{
	char *input;
	t_token *input_list;

	printf("Welcome to the Parsing Test!\n");
	printf("Type commands, use up/down arrows for history, or press Ctrl+D to exit.\n\n");
	while (1)
	{
		input = readline("minishell> ");
		if (input == NULL)
		{
			printf("exit\n");
			break;
		}
		if (input[0] != '\0')
		{
			add_history(input);
			printf("Ready to parse token: [%s]\n", input);
		}
		input_list = NULL;
		if (build_token(&input_list, input) == 1)
			free_tokens(&input_list);
		free(input);
	}
	return (0);
}