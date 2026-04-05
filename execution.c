/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/05 10:31:33 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    pipe_process(t_cmd *command_list, char **envp, int curr_pipe[2], int last_pipe)
{
    
}

void    execution(t_cmd *command_list, char **envp)
{
    int     last_pipe;
    int     curr_pipe[2];
    
    if (!command_list)
        return ;
    last_pipe = -1;
    while (command_list)
    {
        if (command_list->next)
            pipe_process(command_list, envp, curr_pipe, last_pipe);
    }
}