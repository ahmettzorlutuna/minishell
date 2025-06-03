/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:33:57 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/09 16:33:57 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_valid_key(const char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	print_env_sorted(t_env *env_list)
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

static int	split_key_value(char *arg, char **key, char **value)
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

int	builtin_export(char **args, t_minishell *minishell)
{
	int		i;
	char	*key;
	char	*value;

	if (!args[1])
	{
		print_env_sorted(minishell->env_list);
		return (0);
	}
	i = 1;
	while (args[i])
	{
		if (split_key_value(args[i], &key, &value) || !is_valid_key(key))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("`: not a valid identifier\n", 2);
		}
		else
		{
			set_env_value(&minishell->env_list, key, value ? value : "");
			update_env_array(minishell);
		}
		free(key);
		free(value);
		i++;
	}
	return (0);
}