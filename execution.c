/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/08 18:28:25 by karmanz          ###   ########.fr       */
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
        if (access(full_path, X_OK) == 0)
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
        perror("Path not Found");
        // need to review exit_program function
        exit_program(command_list);
    }
    if (execve(path, command_list->commands, envp) == -1)
    {
        perror("Execve Failure");
        // need to review exit_program function
        exit_program(command);
    }
}

void    pipe_process(t_cmd *command_list, char **envp, int curr_pipe[2], int last_pipe)
{

    if (last_pipe != -1)
        dup2(last_pipe, STDIN_FILENO);
    if (command_list->next)
        dup2(curr_pipe[1], STDOUT_FILENO);
    close(curr_pipe[1]);
    close(curr_pipe[0]);
    close(last_pipe);
    execute_command(command_list, envp);
}

// Start of execution
void    reading_commands(t_cmd *command_list, char **envp)
{
    int     last_pipe;
    int     curr_pipe[2];
    pid_t   child;
    
    if (!command_list)
        return ;
    last_pipe = -1;
    while (command_list)
    {
        if (command_list->next)
            pipe(curr_pipe);
        child = fork();
        if (child == 0)
            pipe_process(command_list, envp, curr_pipe, last_pipe);
        if (last_pipe != -1)
            close(last_pipe);
        if (command_list->next)
        {
            close(cur_token[1]);
            last_pipe = curr_pipe[0];
        }
        else
            last_pipe = -1;
        command_list = command_list->next;
    }

    // Need to wait for child processes to finish
}