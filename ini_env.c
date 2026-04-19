/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 11:45:04 by cocozhu           #+#    #+#             */
/*   Updated: 2026/04/19 22:41:41 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_env(t_env *env_list, int start, int *i)
{
	char	*replace;
	int		i;

	i = 0;
	
	
	
}

char	*find_env_value(t_env *env_list, char *replace)
{
	t_env	*cur;
	int len;
	
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
int	find_sep(char *cur)
{
	int i;

	i = 0;
	while (cur[i])
	{
		if (cur[i] == '=')
			return (i);
		i++;
	}
	return (0);
}

t_env	*creat_envp_node(char *envp_str)
{
	t_env *cur;
	int sep;

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
		cur->value = ft_substr(envp_str, sep + 1, (ft_strlen(envp_str) - sep - 1));	
	}
	return (cur);
}

t_env	*build_envp(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*new_node;
	int i;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		new_node = creat_envp_node(envp[i]);
		if (head == NULL)
		{
			head = new_node;
			tail = head;
		}
		else
		{
			tail->next = new_node;
			tail = tail->next;
		}
		i++;
	}
	return (head);
}
