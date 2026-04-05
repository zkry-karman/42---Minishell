/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:07:38 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/05 17:57:01 by cocozhu          ###   ########.fr       */
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
    char    *value;
    int     type;
    struct s_token  *next;
}  t_token;

int 	build_token(t_token **input_list, char *input);
int	    append_node(t_token **input_list, char *token);
char	*extract_token(char *input, int *index);

int 	is_space(char c);
void    free_tokens(t_token **tokens);

/*typedef struct s_cmd
{
    char    **commands; // I dont know if the command and flag should be character pointers or not. I will leave them like this for now. My logic is if you split them and pass them as strings, then i will get the path to the command when i execute it. 
    t_cmd   *next;
}   t_cmd ;*/

#endif