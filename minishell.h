/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:07:38 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/12 15:10:48 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct  s_shell
{
    char    **envp_copy;
    t_cmd   *command_list; //pointer to the head of the list
    int     last_exit_val;
}   t_shell;

typedef struct s_cmd
{
    char    **commands;
    t_cmd   *next;
    t_shell     *shell;
}   t_cmd;
//pour parsing
/*typedef struct s_token
{
    char    *value;
    int     type;
    struct s_token  *next;
}  t_token;
*/
int 	build_token(t_token **input_list, char *input);
int	    append_node(t_token **input_list, char *token);
char	*extract_token(char *input, int *i);
char	*extract_word(char *input, int *i);
char	*extract_quote(char *input, int *i);

int 	is_space(char c);
void    free_tokens(t_token **tokens);

#endif