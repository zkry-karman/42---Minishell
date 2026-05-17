/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 18:09:59 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/05/17 15:55:07 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*cur;
	t_token	*tmp;

	if (!tokens || !*tokens)
		return ;
	cur = *tokens;
	while (cur != NULL)
	{
		tmp = cur->next;
		if (cur->value != NULL)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	*tokens = NULL;
}

void	free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_redirs(t_redir **redirs)
{
	t_redir	*tmp;
	t_redir	*cur;

	if (!redirs || !*redirs)
		return ;
	cur = *redirs;
	while (cur != NULL)
	{
		tmp = cur->next;
		if (cur->file != NULL)
			free (cur->file);
		free(cur);
		cur = tmp;
	}
	*redirs = NULL;
}

void	free_cmds(t_cmd **cmds)
{
	t_cmd	*temp;
	t_cmd	*cur;

	if (!cmds || !*cmds)
		return ;
	cur = *cmds;
	while (cur)
	{
		temp = cur->next;
		if (cur->args)
			free_array(cur->args);
		if (cur->redirs)
			free_redirs(&(cur->redirs));
		if (cur->infile > 2)
			close_if_non_standard_in_out_file(&cur->infile, NULL);
		if (cur->outfile > 2)
			close_if_non_standard_in_out_file(NULL, &cur->outfile);
		free(cur);
		cur = temp;
	}
	*cmds = NULL;
}

void	free_env(t_env **env_list)
{
	t_env	*tmp;
	t_env	*cur;

	if (!env_list || !*env_list)
		return ;
	cur = *env_list;
	while (cur != NULL)
	{
		tmp = cur->next;
		if (cur->key != NULL)
			free(cur->key);
		if (cur->value != NULL)
			free(cur->value);
		free(cur);
		cur = tmp;
	}
	*env_list = NULL;
}
