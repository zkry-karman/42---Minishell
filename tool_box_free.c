/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 18:09:59 by kzhu@studen       #+#    #+#             */
/*   Updated: 2026/04/28 14:17:55 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **tokens)
{
	t_token	*current;
	t_token	*next;

	if (!tokens || !*tokens)
		return ;
	current = *tokens;
	while (current != NULL)
	{
		next = current->next;
		if (current->value != NULL)
			free(current->value);
		free(current);
		current = next;
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

void free_redirs(t_redir *redirs)
{
	t_redir *tmp;
	while (redirs != NULL)
	{
		tmp = redirs->next;
		if (redirs->file != NULL)
			free (redirs->file);
		free(redirs);
		redirs = tmp;
	}
}

void    free_cmds(t_cmd *cmds)
{
    t_cmd   *temp;

    while (cmds)
    {
        temp = cmds;
        cmds = cmds->next;
        if (temp->args)
            free_array(temp->args);
        if(temp->redirs)
            free_redirs(temp->redirs);
        if (temp->infile > 0)
            close (temp->infile);
        if (temp->outfile > 1)
            close (temp->outfile);
        free(temp);
    }
}

void	free_env(t_env *env_list)
{
	t_env	*tmp;

	while (env_list != NULL)
	{
		tmp = env_list->next;
		if (env_list->key != NULL)
			free(env_list->key);
		if (env_list->value != NULL)
			free(env_list->value);
		free(env_list);
		env_list = tmp;
	}
}

