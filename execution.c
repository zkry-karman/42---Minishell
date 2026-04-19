/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/19 15:06:04 by karmanz          ###   ########.fr       */
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

void    execute_command(t_shell *shell)
{
    char    *path;
    char    **envp_arr;

    if (!shell->cmds || !shell->cmds->args[0])
        return ;
    envp_arr = envp_list_to_arr(shell);
    path = get_path(shell->cmds->args[0], envp_arr);
    if (!path)
    {
        free(envp_arr);
        ft_putstr_fd(shell->cmds->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit_program(shell, 127);
    }
    if (execve(path, shell->cmds->args, envp_arr) == -1)
    {
        free(envp_arr);
        perror("Execve Failure");
        // Cannot use -1 here
        exit_program(shell, -1);
    }
}

void    pipe_process(t_shell *shell, int curr_pipe[2], int last_pipe)
{

    if (last_pipe != -1)
        dup2(last_pipe, STDIN_FILENO);
    if (shell->cmds->next)
        dup2(curr_pipe[1], STDOUT_FILENO);
    if (shell->cmds->next)
    {
        close(curr_pipe[1]);
        close(curr_pipe[0]);
    }
    if (last_pipe != -1)
        close(last_pipe);
    execute_command(shell);
}

// Start of execution
void    reading_commands(t_shell *shell)
{
    int     last_pipe;
    int     curr_pipe[2];
    pid_t   *children;
    int     i;
    t_shell     curr_cmd;
    
    if (!shell)
        return ;
    curr_cmd = shell->cmds;
    last_pipe = -1;
    children = malloc(sizeof(pid_t) * ft_lstsize(shell->cmds));
    if (!children)
        return ;
    i = 0;
    while (curr_cmd)
    {
        if (curr_cmd->next)
            pipe(curr_pipe);
        children[i] = fork();
        if (children[i] == 0)
            pipe_process(shell, curr_pipe, last_pipe);
        if (last_pipe != -1)
            close(last_pipe);
        if (curr_cmd->next)
        {
            close(curr_pipe[1]);
            last_pipe = curr_pipe[0];
        }
        else
            last_pipe = -1;
        i++;
        curr_cmd = curr_cmd->next;
    }
    i = 0;
    while (i < command_count)
    {
        waitpid(children[i], NULL, 0);
        i++;
    }
    free(children);
}