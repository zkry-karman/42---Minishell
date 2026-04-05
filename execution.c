/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/05 12:08:45 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *env_path(char **envp)
{
    int     i;
    
    i = 0;
    while (envp[i])
    {
        if (ft_strcmp(envp[i], "PATH=", 5) == 0)
            return (envp[i] + 5);
        i++;
    }
    return (NULL);
}

char    *get_path(char *command, char **envp)
{
    char    *path_str;
    char    **paths;
    char    **full_path;
    char    *temp;
    int     i;

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
        if (acces(full_path, X_OK) == 0)
        {
            // Free paths;
            return (full_path);
        }
        free(full_path);
        i++;
    }
    // Free paths;
    return (NULL);
}

void    execute_command(t_cmd *command_list, char **envp)
{
    char    *path;

    path = get_path(command_list->commands[0], envp);
    if (!path)
    {
        //exit error;
    }
    if (execve(path, command_list->commands, envp) == -1)
    {
        //exit error;
    }
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