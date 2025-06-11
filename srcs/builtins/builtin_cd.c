/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:33:22 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/09 16:33:23 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_home_path(t_env *env_list)
{
	return (get_env_value(env_list, "HOME"));
}

static char	*get_oldpwd_path(t_env *env_list)
{
	return (get_env_value(env_list, "OLDPWD"));
}

static void	print_cd_error(char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

int	builtin_cd(char **args, t_minishell *minishell)
{
	char	*target_path;
	char	cwd[PATH_MAX];

	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (!args[1] || !ft_strcmp(args[1], "~"))
		target_path = get_home_path(minishell->env_list);
	else if (!ft_strcmp(args[1], "-"))
	{
		target_path = get_oldpwd_path(minishell->env_list);
		if (!target_path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (1);
		}
		ft_putendl_fd(target_path, 1);
	}
	else
		target_path = args[1];
	if (!target_path || chdir(target_path) != 0)
	{
		print_cd_error(args[1]);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		set_env_value(&minishell->env_list, "OLDPWD", get_env_value(minishell->env_list, "PWD"));
		set_env_value(&minishell->env_list, "PWD", cwd);
	}
	return (0);
}