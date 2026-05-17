/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu@student.42.fr <kzhu>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 14:22:32 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/17 21:10:17 by kzhu@student.42.f###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_to_env(t_shell *shell, char *key, char *value)
{
	t_env	*new_env;
	t_env	*tmp;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = NULL;
	if (!shell->env_list)
		shell->env_list = new_env;
	else
	{
		tmp = shell->env_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_env;
	}
}

void	process_arg(t_shell *shell, char *arg)
{
	t_env	*target_node;
	char	*arg_copy;
	char	*equal_sign;
	char	*key;
	char	*value;

	arg_copy = ft_strdup(arg);
	if (!arg_copy)
		return ;
	equal_sign = ft_strchr(arg_copy, '=');
	if (!equal_sign)
		return (free(arg_copy));
	*equal_sign = '\0';
	key = arg_copy;
	value = equal_sign + 1;
	target_node = get_env_node(shell, key);
	if (target_node)
		replace_env_value(target_node, value);
	else
		add_to_env(shell, key, value);
	free (arg_copy);
}

int	is_valid_arg(char *arg)
{
	int	i;

	i = 0;
	if (!arg || arg[0] == '\0')
		return (0);
	if (!(ft_isalpha(arg[i]) || arg[i] == '_'))
		return (0);
	while (arg[i] && arg[i] != '=')
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(t_shell *shell, char **args)
{
	int	i;
	int	exit_status;

	exit_status = 0;
	if (!args[1])
		return (show_env(shell->env_list));
	i = 1;
	while (args[i])
	{
		if (is_valid_arg(args[i]))
			process_arg(shell, args[i]);
		else
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			exit_status = 1;
		}
		i++;
	}
	return (exit_status);
}
