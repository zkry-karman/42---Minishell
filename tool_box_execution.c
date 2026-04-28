/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool_box_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 12:27:28 by cocozhu           #+#    #+#             */
/*   Updated: 2026/04/28 16:43:55 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *env_path(char **envp)
{
    int     i;
    
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return (envp[i] + 5);
        i++;
    }
    return (NULL);
}

int     envp_size(t_env *envp)
{
    int     i;
    
    i = 0;
    while (envp)
    {
        i++;
        envp = envp->next;
    }
    return (i);
}

char    **envp_list_to_arr(t_shell *shell)
{
    char    **arr;
    t_env   *curr;
    int     i;
    char    *temp;

    curr = shell->env_list;
    arr = malloc(sizeof(char *) * envp_size(curr) + 1);
    if (!arr)
        return (NULL);
    i = 0;
    while (curr)
    {
        temp = ft_strjoin(curr->key, "=");
        arr[i] = ft_strjoin(temp, curr->value);
        free(temp);
        i++;
        curr = curr->next;
    }
    arr[i] = NULL;
    return (arr);
}

int     command_count(t_cmd *cmds)
{
    int     i;
    t_cmd   *tmp;
    
    i = 0;
    tmp = cmds;
    while (tmp)
    {
        i++;
        tmp = tmp->next;
    }
    return (i);
}
