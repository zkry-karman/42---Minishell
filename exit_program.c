/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_program.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 11:24:06 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/24 20:38:36 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_envp_list(t_env *env)
{
    t_env   *temp;

    while (env)
    {
        temp = env;
        env = env->next;
        if (temp->key)
            free(temp->key);
        if (temp->value)
            free(temp->value);
        free(temp);
    }
}

void    free_redirs(t_redir *redirs)
{
    t_redir *temp;

    while (redirs)
    {
        temp = redirs;
        redirs = redirs->next;
        if (temp->file)
            free(temp->file);
        free(temp);
    }
}

void    free_cmds(t_cmd *cmds)
{
    t_cmd   *temp;

    while (cmds)
    {
        temp = cmds;
        cmds = cmds->next;
        if (temp->args)
            free_dbl_pointer(temp->args);
        if(temp->redirs)
            free_redirs(temp->redirs);
        if (temp->infile > 0)
            close (temp->infile);
        if (temp->outfile > 1)
            close (temp->outfile)
        free(temp);
    }
}

void    exit_program(t_shell *shell, int exit_code)
{
    if (shell)
    {
        if (shell->cmds)
            free_cmds(shell->cmds);
        if (shell->env_list);
            free_envp_list(shell->env_list);
    }
    exit (exit_code);
}