/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 11:45:04 by cocozhu           #+#    #+#             */
/*   Updated: 2026/05/13 18:07:26 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(t_shell *shell, char *final, int *i)
{
	int		start;
	char	*replace;
	char	*result;

	start = *i;
	while (ft_isdigit(final[*i]) || ft_isalpha(final[*i])
		|| final[*i] == '_')
		(*i)++;
	replace = ft_substr(final, start, (*i) - start);
	result = find_env_value(shell->env_list, replace);
	free(replace);
	return (result);
}

char	*extract_env(t_shell *shell, char *final, int *i)
{
	(*i)++;
	if (final[*i] == ' ' || final[*i] == '\0'
		|| final[*i] == '\"')
		return (ft_strdup("$"));
	if (final[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	if (ft_isdigit(final[*i]))
	{
		(*i)++;
		return (ft_strdup(""));
	}
	if (ft_isalpha(final[*i]) || final[*i] == '_')
		return (get_value(shell, final, i));
	return (ft_strdup("$"));
}

char	*find_env_value(t_env *env_list, char *replace)
{
	t_env	*cur;
	int		len;

	cur = env_list;
	len = ft_strlen(replace);
	while (cur != NULL)
	{
		if (ft_strncmp(cur->key, replace, len) == 0 && cur->key[len] == '\0')
			return (ft_strdup(cur->value));
		cur = cur->next;
	}
	return (ft_strdup(""));
}

t_env	*create_envp_node(char *envp_str)
{
	t_env	*cur;
	int		sep;

	cur = malloc(sizeof(t_env));
	if (cur == NULL)
		return (NULL);
	ft_memset(cur, 0, sizeof(t_env));
	sep = find_sep(envp_str);
	if (sep == 0)
	{
		cur->key = ft_strdup(envp_str);
		cur->value = ft_strdup("");
	}
	else
	{
		cur->key = ft_substr(envp_str, 0, sep);
		cur->value = ft_substr(envp_str, sep + 1,
				(ft_strlen(envp_str) - sep - 1));
	}
	if (!cur->key || !cur->value)
		return (free(cur->key), free(cur->value), free(cur), NULL);
	return (cur);
}

t_env	*build_envp(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*new_node;
	int		i;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		new_node = create_envp_node(envp[i]);
		if (!new_node)
			return (free_env(&head), NULL);
		if (head == NULL)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		i++;
	}
	return (head);
}
