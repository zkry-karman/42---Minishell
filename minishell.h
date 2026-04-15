/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:07:38 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/15 14:01:31 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

//pour parsing
typedef struct s_token
{
    char    **commands;
    t_cmd   *next;
    t_shell     *shell;
}   t_cmd;

int 	build_token(t_token **input_list, char *input);
int	    append_node(t_token **input_list, char *token);
char	*extract_token(char *input, int *i);
char	*extract_word(char *input, int *i);
char	*extract_quote(char *input, int *i);
void    reading_commands(t_cmd *command_list);
int 	is_space(char c);
void    free_tokens(t_token **tokens);

/*typedef struct s_cmd
{
    char    **commands; //Just split each command into a **arr instead of separate components. I need to pass a ** to execve to execute the function
    t_cmd   *next;
}   t_cmd ;
*/
#endif