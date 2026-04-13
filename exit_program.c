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

    if (command_list)
        ft_lstclear(&command_list, del(void *));
    exit (exit_code);
}