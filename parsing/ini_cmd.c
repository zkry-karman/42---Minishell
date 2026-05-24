/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 11:55:10 by cocozhu           #+#    #+#             */
/*   Updated: 2026/05/24 15:22:40 by cocozhu          ###   ########.fr       */
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
	if (new_node->file == NULL)
		return (free(new_node), 1);
	new_node->quoted = (*cur)->next->quoted;
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
	char	**split;
	int		j;

	if (ft_strchr((*cur)->value, ' '))
	{
		split = ft_split((*cur)->value, ' ');
		j = 0;
		while (split && split[j])
		{
			args[*i] = restore_spaces(ft_strdup(split[j]));
			(*i)++;
			j++;
		}
		free_array(split);
	}
	else
	{
		args[*i] = restore_spaces(ft_strdup((*cur)->value));
		(*i)++;
	}
	(*cur) = (*cur)->next;
}

int	count_words_mini(t_token *token)
{
	int		total;
	int		i;
	char	**split;

	total = 0;
	while (token && token->type != TOKEN_PIPE)
	{
		if (token->type != TOKEN_WORD)
			token = token->next;
		else if (ft_strchr(token->value, ' '))
		{
			split = ft_split(token->value, ' ');
			i = 0;
			while (split && split[i])
				i++;
			total = i + total;
			free_array(split);
		}
		else
			total++;
		if (token)
			token = token->next;
	}
	return (total);
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
				return (free_redirs(&(cmd->redirs)),
					free_array(cmd->args), free(cmd), NULL);
		}
		else
			create_args(cmd->args, &arg_count, cur);
	}
	return (cmd->args[arg_count] = NULL, cmd);
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
			return (free_cmds(&head), 1);
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
