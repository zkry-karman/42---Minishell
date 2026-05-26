/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 11:11:06 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/26 17:48:48 by zkarman          ###   ########.fr       */
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

	path_str = env_path(envp);
	if (!path_str || path_str[0] == '\0')
		return (check_access(command));
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

void	initialize_pipe(t_pipe *p)
{
	p->last_pipe = -1;
	p->curr[0] = -1;
	p->curr[1] = -1;
	p->i = 0;
}

void	fork_failure(t_shell *shell, t_cmd *curr, t_pipe *p)
{
	perror("minishell: fork");
	shell->exit_status = 1;
	if (curr->next)
	{
		close(p->curr[1]);
		close(p->curr[0]);
	}
	if (p->last_pipe != -1)
	{
		close(p->last_pipe);
		p->last_pipe = -1;
	}
}
