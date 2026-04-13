/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_program.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 11:24:06 by karmanz           #+#    #+#             */
/*   Updated: 2026/04/10 11:40:48 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    exit_program(t_cmd *command_list, int exit_code)
{

    if (command_list && command_list->shell)
    {
        if (command_list->shell->envp_copy)
            free_dbl_pointer(command_list->shell->envp_copy);
        ft_lstclear(&command_list->shell->command_list, del(void *));
        free(command_list->shell);
    }
    exit (exit_code);
}