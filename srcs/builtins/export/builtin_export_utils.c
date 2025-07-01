/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 23:30:52 by azorlutu          #+#    #+#             */
/*   Updated: 2025/06/30 23:30:53 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env_sorted(t_env *env_list)
{
	t_env	*sorted;
	t_env	*tmp;

	sorted = sort_env_list(env_list);
	tmp = sorted;
	while (tmp)
	{
		if (tmp->key && ft_strcmp(tmp->key, "_") != 0)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(tmp->key, 1);
			if (tmp->value)
			{
				ft_putstr_fd("=\"", 1);
				ft_putstr_fd(tmp->value, 1);
				ft_putstr_fd("\"\n", 1);
			}
			else
				ft_putchar_fd('\n', 1);
		}
		tmp = tmp->next;
	}
	free_env_list(sorted);
}

int	split_key_value(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (equal)
	{
		*key = ft_strndup(arg, equal - arg);
		if (!*key)
			return (1);
		*value = ft_strdup(equal + 1);
		if (!*value)
		{
			free(*key);
			return (1);
		}
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
		if (!*key)
			return (1);
	}
	return (0);
}

int	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("`: not a valid identifier\n", 2);
	return (1);
}

int	handle_no_args(t_minishell *minishell)
{
	print_env_sorted(minishell->env_list);
	return (0);
}

void	set_env_and_update(t_minishell *minishell, char *key, char *value)
{
	if (value)
		set_env_value(&minishell->env_list, key, value);
	else
		set_env_value(&minishell->env_list, key, "");
	update_env_array(minishell);
}
