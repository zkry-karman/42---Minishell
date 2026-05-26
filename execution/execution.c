/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 12:03:52 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/26 19:04:13 by zkarman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_path_env(char **envp)
{
	int		i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	execute_command(t_shell *shell, t_cmd *cmd)
{
	char		*path;
	char		**envp_arr;
	struct stat	path_stat;

	if (!cmd->args[0])
		exit_program(shell, 0);
	if (cmd->args[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		exit_program(shell, 127);
	}
	envp_arr = envp_list_to_arr(shell);
	if (ft_strchr(cmd->args[0], '/'))
		path = ft_strdup(cmd->args[0]);
	else
		path = get_path(cmd->args[0], envp_arr);
	if (!path)
		exit_no_path(shell, envp_arr, cmd);
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
		exit_no_access(shell, path, envp_arr);
	if (execve(path, cmd->args, envp_arr) == -1)
	{
		free(path);
		exit_execve_failure(shell, envp_arr, cmd);
	}
}

void	pipe_process(t_shell *shell, t_cmd *cmd, t_pipe *p)
{
	if (p->last_pipe != -1)
	{
		dup2(p->last_pipe, STDIN_FILENO);
		close(p->last_pipe);
		p->last_pipe = -1;
	}
	if (cmd->next)
	{
		dup2(p->curr[1], STDOUT_FILENO);
		close(p->curr[1]);
		close(p->curr[0]);
		p->curr[1] = -1;
		p->curr[0] = -1;
	}
	if (check_file_descriptors(cmd) == -1)
		exit_fd_failure(shell, cmd, p);
	verify_stds(cmd);
	if (is_built_in_command(cmd->args[0]))
		run_built_in_and_exit(shell, cmd, p);
	else
		execute_command(shell, cmd);
}

int	execute_system_command(t_shell *shell, t_cmd *curr, t_pipe *p)
{
	if (curr->next)
	{
		if (pipe(p->curr) == -1)
		{
			perror("minishell: pipe");
			shell->exit_status = 1;
			return (1);
		}
	}
	p->children[p->i] = fork();
	if (p->children[p->i] == -1)
		return (fork_failure(shell, curr, p), 1);
	if (p->children[p->i] == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		pipe_process(shell, curr, p);
	}
	check_for_next_pipe(p, curr);
	close_if_non_standard_in_out_file(&curr->infile, &curr->outfile);
	return (0);
}

// Start of execution
void	reading_commands(t_shell *shell)
{
	t_pipe	p;
	t_cmd	*curr_cmd;
	int		stdout_dup;

	if (!shell->cmds)
		return ;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	initialize_pipe(&p);
	shell->pipe_processes = &p;
	initialize_children(shell, &p);
	if (!p.children)
		return (children_failure(shell));
	if (check_heredocs(shell))
		return (heredoc_failure(shell, &p));
	curr_cmd = shell->cmds;
	stdout_dup = 0;
	if (loop_cmds(shell, &p, curr_cmd, stdout_dup) != 0)
	{
		if (p.last_pipe != -1)
			close(p.last_pipe);
		return (clean_up_children(shell, &p));
	}
	check_for_next_pipe(&p, NULL);
	clean_up_children(shell, &p);
}
