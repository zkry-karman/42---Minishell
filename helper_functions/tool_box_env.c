/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 12:27:28 by cocozhu           #+#    #+#             */
/*   Updated: 2026/05/23 16:20:40 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_env_node(t_shell *shell, char *wanted_path)
{
	t_env	*tmp;
	size_t	len;

	if (!shell || !wanted_path)
		return (NULL);
	tmp = shell->env_list;
	len = ft_strlen(wanted_path);
	while (tmp)
	{
		if (ft_strncmp(tmp->key, wanted_path, len) == 0
			&& ft_strlen(tmp->key) == len)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*env_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

int	envp_size(t_env *envp)
{
	int	i;

	i = 0;
	while (envp)
	{
		i++;
		envp = envp->next;
	}
	return (i);
}

static char	*link_and_join(t_env *node)
{
	char	*temp;
	char	*value_to_join;
	char	*result;

	temp = ft_strjoin(node->key, "=");
	if (!temp)
		return (NULL);
	if (node->value)
		value_to_join = node->value;
	else
		value_to_join = "";
	result = ft_strjoin(temp, value_to_join);
	free(temp);
	return (result);
}

char	**envp_list_to_arr(t_shell *shell)
{
	char	**arr;
	t_env	*curr;
	int		i;

	curr = shell->env_list;
	arr = malloc(sizeof(char *) * (envp_size(curr) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (curr)
	{
		arr[i] = link_and_join(curr);
		if (!arr[i])
			return (free_array(arr), NULL);
		i++;
		curr = curr->next;
	}
	arr[i] = NULL;
	return (arr);
}
