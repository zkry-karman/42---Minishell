/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 12:27:28 by cocozhu           #+#    #+#             */
/*   Updated: 2026/05/22 16:04:08 by karmanz          ###   ########.fr       */
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

char	**envp_list_to_arr(t_shell *shell)
{
	char	**arr;
	t_env	*curr;
	int		i;
	char	*temp;
	char	*value_to_join;

	curr = shell->env_list;
	arr = malloc(sizeof(char *) * (envp_size(curr) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (curr)
	{
		temp = ft_strjoin(curr->key, "=");
		if (!temp)
			return (free_array(arr), NULL);
		if (curr->value)
			value_to_join = curr->value;
		else
			value_to_join = "";
		arr[i] = ft_strjoin(temp, value_to_join);
		if (!arr[i])
			return (free(temp), free_array(arr), NULL);
		free(temp);
		i++;
		curr = curr->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	replace_env_value(t_env *node, char *new_val)
{
	if (!node || !new_val)
		return ;
	free(node->value);
	node->value = ft_strdup(new_val);
}
