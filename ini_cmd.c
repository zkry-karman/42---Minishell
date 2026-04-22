/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ini_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 11:55:10 by cocozhu           #+#    #+#             */
/*   Updated: 2026/04/22 23:17:51 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	build_cmd(t_shell *shell)
{
	t_token *cur;

	cur = shell->input_list;
	while (cur != NULL)
	{
		
		while (is_space(input[i]) == 1)
			i++;
		if (input[i] == '\0')
			break ; 
		cur_token = extract_token(shell, input, &i);
		if (cur_token == NULL)
			return (1);
		append_node(&(shell->cmds), cur);
	}
	return (0);
}