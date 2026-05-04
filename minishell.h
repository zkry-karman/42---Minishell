/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:07:38 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/04 11:15:56 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>

extern int	g_status;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_IN,
	TOKEN_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				quoted;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	int				quoted;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	int				infile;
	int				outfile;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_pipe
{
	int		curr[2];
	int		last_pipe;
	int		i;
	pid_t	*children;
}	t_pipe;

typedef struct s_shell
{
	t_env	*env_list;
	t_cmd	*cmds;
	t_token	*input_list;
	int		exit_status;
}	t_shell;

t_token_type	identify_type(char *value);

void			ini_shell(t_shell *shell, char	**envp);
void			handle_sigint(int sig);

char			*find_env_value(t_env *env_list, char *replace);
t_env			*create_envp_node(char *envp_str);
t_env			*build_envp(char **envp);
char			*extract_env(t_shell *shell, char *final, int *i);
char			*get_value(t_shell *shell, char *final, int *i);

int				build_token(t_shell *shell, char *input);
char			*extract_token(t_shell *shell, char *input, int *i, int *qted);
char			*extract_word(t_shell *shell, char *input, int *i);
char			*extract_quote(t_shell *shell, char *input, int *i);
char			*extract_d_quote(t_shell *shell, char *input, int *i);
char			*extract_operator(char *input, int *i);
int				append_node(t_token **input_list, char *token, int quoted);
char			*join_and_free(char *s1, char *s2);
int				syntax_checker(t_token *tokens);

int				build_redir_node(t_redir **redir, t_token **cur);
void			create_args(char **args, int *i, t_token **cur);
int				count_words_mini(t_token *token);
t_cmd			*create_cmd_node(t_token **cur);
int				build_cmds(t_shell *shell);

int				is_space(char c);
int				is_delimiter(char c);
int				find_sep(char *cur);
void			free_tokens(t_token **tokens);
void			free_array(char **arr);
void			free_redirs(t_redir *redirs);
void			free_cmds(t_cmd *cmds);
void			free_env(t_env *env_list);

void			reading_commands(t_shell *shell);
char			**envp_list_to_arr(t_shell *shell);
int				check_file_descriptors(t_cmd *cmd);
void			check_heredocs(t_shell *shell);
int				handle_heredoc(t_shell *shell, t_redir *curr_redir);
char			*expand_heredoc(t_shell *shell, char *line);
void			exit_program(t_shell *shell, int exit_code);
int				command_count(t_cmd *cmds);
char			*env_path(char **envp);
int				ft_strcmp(const char *s1, const char *s2);
int				is_built_in_command(char *cmd);
int				execute_built_in_command(t_shell *shell, t_cmd *cmd);
int				change_dir(t_shell *shell, char **args);
t_env			*get_env_node(t_shell *shell, char *wanted_path);
void			replace_env_value(t_env *node, char *new_val);
int				echo(char **args);
int				show_env(t_env *env_list);
int				check_exit(t_shell *shell, char **args);
int				is_valid_arg(char *arg);
int				ft_export(t_shell *shell, char **args);
int				print_cwd(t_shell *shell, char **args);
int				ft_unset(t_shell *shell, char **args);
void			close_if_non_standard_in_out_file(int infile, int outfile);
void			check_for_next_pipe(t_pipe *p, t_cmd *curr_cmd);
char			*get_path(char *command, char **envp);

#endif
