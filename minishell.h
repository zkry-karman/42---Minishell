/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:07:38 by zkarman           #+#    #+#             */
/*   Updated: 2026/04/23 19:18:41 by kzhu@student.42.f###   ########.fr       */
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
    char            *key;
    char            *value;
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
    char            *value;
    t_token_type    type;
    struct s_token  *next;
}   t_token;

typedef struct s_redir
{
    t_token_type    type;
    char            *file;
    struct s_redir  *next;
}   t_redir;

typedef struct s_cmd
{
    char            **args;
    t_redir         *redirs;
	int				infile;
	int				outfile;
    struct s_cmd    *next;
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
char	*extract_d_quote(t_shell *shell, char *input, int *i);
char	*extract_operator(char *input, int *i);
int	    append_node(t_token **input_list, char *token);
t_token_type identify_type(char *value);
char	*join_and_free(char *s1, char *s2);

char	*find_env_value(t_env *env_list, char *replace);
t_env	*create_envp_node(char *envp_str);
t_env	*build_envp(char **envp);
char	*extract_env(t_shell *shell, char *final, int *i);
char	*get_value(t_shell *shell, char *final, int *i);

int 	is_space(char c);
int		is_delimiter(char c);
int	    find_sep(char *cur);
void    free_tokens(t_token **tokens);
void	free_array(char **arr);
void free_redirs(t_redir *redirs);
void	free_cmds(t_cmd *cmds);
void	free_env(t_env *env_list);

int	build_redir_node(t_redir **redir, t_token **cur);
void	create_args(char **args, int *i, t_token **cur);
int count_words_mini(t_token *token);
t_cmd	*create_cmd_node(t_token **cur);
int	build_cmds(t_shell *shell);

void    reading_commands(t_shell **shell);

#endif