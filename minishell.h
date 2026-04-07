/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:07:38 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/07 15:49:47 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_cmd
//pour parsing
typedef struct s_token
{
    char    *value;
    int     type;
    struct s_token  *next;
}  t_token;

int 	build_token(t_token **input_list, char *input);
int	    append_node(t_token **input_list, char *token);
char	*extract_token(char *input, int *i);
char	*extract_word(char *input, int *i);
char	*extract_quote(char *input, int *i);

int 	is_space(char c);
void    free_tokens(t_token **tokens);

/*typedef struct s_cmd
{
    char    **commands; //Just split each command into a **arr instead of separate components. I need to pass a ** to execve to execute the function
    t_cmd   *next;
}   t_cmd ;

#endif