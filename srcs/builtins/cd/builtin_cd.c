/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:33:22 by azorlutu          #+#    #+#             */
/*   Updated: 2025/08/01 21:43:56 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	char	*get_cd_target(char *arg, t_minishell *minishell)
{
	if (!arg)
		return (get_env_value(minishell->env_list, "HOME"));
	if (ft_strcmp(arg, "-") == 0)
		return (get_env_value(minishell->env_list, "OLDPWD"));
	return (arg);
}

static	int	handle_cd_null(char *arg, t_minishell *minishell)
{
	char	*home;

	if (arg && ft_strcmp(arg, "-") == 0)
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
	else
	{
		home = get_env_value(minishell->env_list, "HOME");
		if (!home)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		else
			print_cd_error(arg);
	}
	return (1);
}

int	builtin_cd(char **args, t_minishell *minishell)
{
	char	*target_path;
	char	cwd[PATH_MAX];

	if (args[1] && args[2])
		return (cd_too_many_args());
	target_path = get_cd_target(args[1], minishell);
	if (!target_path)
		return (handle_cd_null(args[1], minishell));
	if (chdir(target_path) != 0)
	{
		print_cd_error(args[1]);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
	{
		set_env_value(&minishell->env_list, "OLDPWD",
			get_env_value(minishell->env_list, "PWD"));
		set_env_value(&minishell->env_list, "PWD", cwd);
	}
	return (0);
}
