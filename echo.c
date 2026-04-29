/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 11:20:20 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/29 11:46:10 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     echo(t_shell *shell, char **args)
{
    int     i;
    int     n_flag;
    
    n_flag = 0;
    i = 1;
    while (args[i] && ft_strcmp(args[i], "-n") == 0)
    {
        n_flag = 1;
        i++;
    }
    while (args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            write (1, " ", 1);
        i++;
    }
    if (n_flag == 0)
        write (1, "\n", 1);
    return (0);
}