/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/05 11:34:01 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    execute_command()
{
    
}

void    pipe_process(t_cmd *command_list, char **envp, int curr_pipe[2], int last_pipe)
{
    pid_t   curr_reading;
    pid_t   curr_writing;

    pipe(curr_pipe);
    curr_reading = fork();
    if (curr_reading == 0)
    {
        dup2(curr_pipe[1], STDOUT_FILENO);
        close(curr_pipe[1]);
        close(curr_pipe[0]);
        // Need to execute function now.
    }
    curr_writing = fork();
    if (curr_writing == 0)
    {
        dup2(curr_pipe[0], STDIN_FILENO);
        close(curr_pipe[1]);
        close(curr_pipe[0]);
        // Need to execution function now. 
    }
    close(curr_pipe[1]);
    close(curr_pipe[0]);
    waitpid(curr_reading, NULL, 0);
    waitpid(curr_writing, NULL, 0);
}

// Start of execution
void    reading_commands(t_cmd *command_list, char **envp)
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