/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 11:55:10 by cocozhu           #+#    #+#             */
/*   Updated: 2026/04/23 19:04:18 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	build_redir_node(t_redir **redir, t_token **cur)
{
	t_redir	*new_node;
	t_redir	*curr;

	new_node = malloc(sizeof(t_redir));
	if (new_node == NULL)
		return (1);
	new_node->type = (*cur)->type;
	new_node->file = ft_strdup((*cur)->next->value);
	new_node->next = NULL;
	if ((*redir) == NULL)
		(*redir) = new_node;
	else
	{
		curr = (*redir);
		while (curr->next != NULL)
			curr = curr->next;
		curr->next = new_node;
	}
	(*cur) = (*cur)->next->next;
	return (0);
}

void	create_args(char **args, int *i, t_token **cur)
{
	char		*cur_word;

	cur_word = ft_strdup((*cur)->value);
	args[*i] = cur_word;
	(*i)++;
	(*cur) = (*cur)->next;
}

int count_words_mini(t_token *token)
{
	t_token	*cur;
	int		operator_count;
	int		total_count;
	int		words_count;
	
	cur = token;
	operator_count = 0;
	total_count = 0;
	while (cur != NULL && cur->type != TOKEN_PIPE)
	{
		total_count++;
		if (cur->type != TOKEN_WORD)
			operator_count++;
		cur = cur->next;
	}
	words_count = total_count - (operator_count * 2);
	if (words_count < 0)
		return (0);
	return (words_count);
}

t_cmd	*create_cmd_node(t_token **cur)
{
	t_cmd	*cmd;
	int		arg_count;

	cmd = malloc(sizeof(t_cmd));
	if (cmd == NULL)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->outfile = 1;
	arg_count = count_words_mini(*cur);
	cmd->args = ft_calloc((arg_count + 1), sizeof(char *));
	if (cmd->args == NULL)
		return (free(cmd), NULL);
	arg_count = 0;
	while ((*cur) != NULL && (*cur)->type != TOKEN_PIPE)
	{
		if ((*cur)->type != TOKEN_WORD)
		{
			if (build_redir_node(&(cmd->redirs), cur) == 1)
				return (free_redirs(cmd->redirs), free_array(cmd->args), free(cmd), NULL);
		}
		else
			create_args(cmd->args, &arg_count, cur);
	}
	cmd->args[arg_count] = NULL;
	return (cmd);
}

int	build_cmds(t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*tail;
	t_cmd	*new_node;
	t_token	*cur;

	head = NULL;
	cur = shell->input_list;
	while (cur != NULL)
	{
		new_node = create_cmd_node(&cur);
		if (!new_node)
			return (free_cmds(head), 1);
		if (head == NULL)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		if (cur != NULL && cur->type == TOKEN_PIPE)
			cur = cur->next;
	}
	shell->cmds = head;
	return (0);
}
