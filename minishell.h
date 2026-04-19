/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:07:38 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/19 18:36:42 by cocozhu          ###   ########.fr       */
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

int 	build_token(t_token **input_list, t_env **envp, char *input);
int	    append_node(t_token **input_list, char *token);
char	*extract_token(char *input, t_env **envp, int *i);
char	*extract_word(char *input, t_env **envp, int *i);
char	*extract_quote(char *input, t_env **envp, int *i);
char	*extract_operator(char *input, int *i);
void    reading_commands(t_cmd *command_list);
int 	is_space(char c);
void    free_tokens(t_token **tokens);
t_token_type identify_type(char *value);

char	*find_env_value(t_env *env_list, char *replace);
int	find_sep(char *cur);
t_env	*creat_envp_node(char *envp_str);
t_env	*build_envp(char **envp);
char	*extract_env(t_env *env_list, int start, int *i);

#endif