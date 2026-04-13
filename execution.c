/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/12 15:34:30 by karmanz          ###   ########.fr       */
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
            free_dbl_pointer(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free_dbl_pointer(paths);
    return (NULL);
}

void    execute_command(t_cmd *command_list)
{
    char    *path;

    path = get_path(command_list->commands[0], command_list->shell->envp_copy);
    if (!path)
    {
        ft_putstr_fd(command_list->commands[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit_program(command_list, 127);
    }
    if (execve(path, command_list->commands, command_list->shell->envp_copy) == -1)
    {
        perror("Execve Failure");
        exit_program(command_list, 127);
    }
}

void    pipe_process(t_cmd *command_list, int curr_pipe[2], int last_pipe)
{

    if (last_pipe != -1)
        dup2(last_pipe, STDIN_FILENO);
    if (command_list->next)
        dup2(curr_pipe[1], STDOUT_FILENO);
    if (command_list->next)
    {
        close(curr_pipe[1]);
        close(curr_pipe[0]);
    }
    if (last_pipe != -1)
        close(last_pipe);
    execute_command(command_list);
}

// Start of execution
void    reading_commands(t_cmd *command_list)
{
    int     last_pipe;
    int     curr_pipe[2];
    pid_t   *children;
    int     i;
    int     command_count;
    
    if (!command_list)
        return ;
    last_pipe = -1;
    command_count = ft_lstsize(command_list);
    children = malloc(sizeof(pid_t) * command_count + 1);
    if (!children)
        return ;
    i = 0;
    while (command_list)
    {
        if (command_list->next)
            pipe(curr_pipe);
        children[i] = fork();
        if (children[i] == 0)
            pipe_process(command_list, curr_pipe, last_pipe);
        if (last_pipe != -1)
            close(last_pipe);
        if (command_list->next)
        {
            close(curr_pipe[1]);
            last_pipe = curr_pipe[0];
        }
        else
            last_pipe = -1;
        i++;
        command_list = command_list->next;
    }
    i = 0;
    while (i < command_count)
    {
        waitpid(children[i], NULL, 0);
        i++;
    }
    free(children);
}