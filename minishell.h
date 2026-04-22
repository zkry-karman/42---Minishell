/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karmanz <karmanz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:07:38 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/22 15:18:41 by karmanz          ###   ########.fr       */
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
    int     infile // Initialize as 1;
    int     outfile // Initialize as 0;
    char    *limiter;
    struct s_cmd *next;
}   t_cmd;

typedef struct s_shell
{
    t_env   *env_list;
    t_cmd   *cmds;
	t_token	*input_list;
    int     exit_status;
}   t_shell;

void	ini_shell(t_shell *shell, char	**envp);

int 	build_token(t_shell *shell, char *input);
char	*extract_token(t_shell *shell, char *input, int *i);
char	*extract_word(t_shell *shell, char *input, int *i);
char	*extract_quote(t_shell *shell, char *input, int *i);
char	*extract_operator(char *input, int *i);
int	    append_node(t_token **input_list, char *token);

int 	is_space(char c);
void    free_tokens(t_token **tokens);
t_token_type identify_type(char *value);

char	*find_env_value(t_env *env_list, char *replace);
int	find_sep(char *cur);
t_env	*creat_envp_node(char *envp_str);
t_env	*build_envp(char **envp);
char	*extract_env(t_shell *shell, char *final, int *i);
char	*get_value(t_shell *shell, char *final, int *i);
int		is_delimiter(char c);

void    reading_commands(t_shell **shell);

#endif