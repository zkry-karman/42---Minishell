/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkarman <zkarman@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 14:22:32 by zkarman           #+#    #+#             */
/*   Updated: 2026/05/03 14:40:13 by zkarman          ###   ########.fr       */
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
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return ;
	*equal_sign = '\0';
	key = arg;
	value = equal_sign + 1;
	target_node = get_env_node(shell, key);
	if (target_node)
		replace_env_value(target_node, value);
	else
		add_to_env(shell, key, value);
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

	if (!args[1])
		return (show_env(shell->env_list));
	i = 1;
	while (args[i])
	{
		if (is_valid_arg(args[i]))
			process_arg(shell, args[i]);
		else
			ft_putstr_fd("minishell: export: invalid identifier\n", 2);
		i++;
	}
	return (0);
}
