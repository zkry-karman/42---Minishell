/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/01 13:14:25 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    wait_children(t_shell *shell, pid_t *child)
{
    int     i;
    int     status;

    i = 0;
    while (i < command_count(shell->cmds))
    {
        waitpid(child[i], &status, 0);
        if (WIFEXITED(status))
            shell->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            shell->exit_status = 128 + WTERMSIG(status);
        i++;
    }
    free(child);
}

void    execute_command(t_shell *shell, t_cmd *cmd)
{
    char    *path;
    char    **envp_arr;

    if (!cmd || !cmd->args || !cmd->args[0])
        return ;
    envp_arr = envp_list_to_arr(shell);
    path = get_path(cmd->args[0], envp_arr);
    if (!path)
    {
        free_array(envp_arr);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit_program(shell, 127);
    }
    if (execve(path, cmd->args, envp_arr) == -1)
    {
        free_array(envp_arr);
        perror("Execve Failure");
        exit_program(shell, 1);
    }
}

void    pipe_process(t_shell *shell, t_cmd *cmd, int curr_pipe[2], int last_pipe)
{
    if (last_pipe != -1)
        dup2(last_pipe, STDIN_FILENO);
    if (cmd->next)
        dup2(curr_pipe[1], STDOUT_FILENO);
    if (check_file_descriptors(cmd) == -1)
        exit_program(shell, 1);
    if (cmd->infile != STDIN_FILENO)
    {
        dup2(cmd->infile, STDIN_FILENO);
        close(cmd->infile);
    }
    if (cmd->outfile != STDOUT_FILENO)
    {
        dup2(cmd->outfile, STDOUT_FILENO);
        close(cmd->outfile);
    }
    if (cmd->next)
    {
        close(curr_pipe[1]);
        close(curr_pipe[0]);
    }
    if (last_pipe != -1)
        close(last_pipe);
    execute_command(shell, cmd);
}

int    execute_system_command(t_shell *shell, t_cmd *curr_cmd, int curr_pipe[2], int last_pipe)
{
    static int  i;
    pid_t   *children;

    i = 0;
    children = malloc(sizeof(pid_t) * command_count(shell->cmds));
    if (!children)
        return (1);
    if (curr_cmd->next)
        pipe(curr_pipe);
    children[i] = fork();
    if (children[i] == 0)
        pipe_process(shell, curr_cmd, curr_pipe, last_pipe);
    check_for_next_pipe(last_pipe, curr_cmd, curr_pipe);
    close_if_non_standard_in_out_file(curr_cmd->infile, curr_cmd->outfile);
    i++;
    return (0);
}

// Start of execution
void    reading_commands(t_shell *shell)
{
    int     last_pipe;
    int     curr_pipe[2];
    pid_t   *children;
    int     i;
    t_cmd     *curr_cmd;
    
    if (!shell)
        return ;
    curr_cmd = shell->cmds;
    last_pipe = -1;
    children = malloc(sizeof(pid_t) * command_count(shell->cmds));
    if (!children)
        return ;
    check_heredocs(shell);
    i = 0;
    while (curr_cmd)
    {
        if (is_built_in_command(curr_cmd->args[0]))
            execute_built_in_command(shell, curr_cmd);
        else
        {
            if (execute_system_command(shell, curr_cmd, curr_pipe, last_pipe))
                return ;
        }
        /*{
            if (curr_cmd->next)
                pipe(curr_pipe);
            children[i] = fork();
            if (children[i] == 0)
                pipe_process(shell, curr_cmd, curr_pipe, last_pipe);
            check_for_next_pipe(last_pipe, curr_cmd, curr_pipe);
            close_if_non_standard_in_out_file(curr_cmd->infile, curr_cmd->outfile);
        }*/
        i++;
        curr_cmd = curr_cmd->next;
    }
    check_for_next_pipe(last_pipe, NULL, NULL);
    wait_children(shell, children);
}
