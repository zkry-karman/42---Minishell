/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp_to_arr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 12:19:30 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/19 15:13:23 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    **envp_list_to_arr(t_shell *shell)
{
    char    **arr;
    t_env   *curr;
    int     i;
    char    *temp;

    curr = shell->env_list;
    arr = malloc(sizeof(char *) * (ft_lstsize(curr) + 1));
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