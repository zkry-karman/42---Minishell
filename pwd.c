/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 13:17:12 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/29 14:17:14 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     print_cwd(t_shell *shell, char **args)
{
    char    cwd[1024];

    (void)args;

    if (getcwd(cwd, sizeof(cwd)))
    {
        printf("%s\n", cwd);
        shell->exit_status = 0;
        return (0);
    }
    else
    {
        perror("minishell: pwd");
        shell->exit_status = 1;
        return (1);
    }
}