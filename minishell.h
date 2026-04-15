/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:07:38 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/15 15:24:54 by karmanz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_env
{
    char    *key;
    char    *value;
    struct s_env    *next;
}   t_env;

typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_IN,
    TOKEN_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
}   t_token_type;

typedef struct s_token
{
    char    *value;
    t_token_type type;
    struct s_token *next;
}   t_token;

typedef struct s_cmd
{
    char    **args;
    int     infile;
    int     outfile;
    struct s_cmd *next;
}   t_cmd;

typedef struct s_shell
{
    t_env   *env_list;
    t_cmd   *cmds;
    int     exit_status;
}   t_shell;

int 	build_token(t_token **input_list, char *input);
int	    append_node(t_token **input_list, char *token);
char	*extract_token(char *input, int *i);
char	*extract_word(char *input, int *i);
char	*extract_quote(char *input, int *i);
void    reading_commands(t_cmd *command_list);
int 	is_space(char c);
void    free_tokens(t_token **tokens);

#endif