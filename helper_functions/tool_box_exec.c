/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 11:11:06 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/04 17:34:39 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char *command, char **envp)
{
	char	*path_str;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (access(command, X_OK) == 0)
		return (ft_strdup(command));
	path_str = env_path(envp);
	if (!path_str)
		return (NULL);
	paths = ft_split(path_str, ':');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, command);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (free_array(paths), full_path);
		free(full_path);
		i++;
	}
	return (free_array(paths), NULL);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0')
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	command_count(t_cmd *cmds)
{
	int		i;
	t_cmd	*tmp;

	i = 0;
	tmp = cmds;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

void	initialize_children(t_shell *shell, t_pipe *p)
{
	int		i;
	
	p->children = malloc(sizeof(pid_t) * command_count(shell->cmds));
	if (!p->children)
		return ;
	i = 0;
	while (i < command_count(shell->cmds))
		p->children[i++] = -1;
	return ;
}