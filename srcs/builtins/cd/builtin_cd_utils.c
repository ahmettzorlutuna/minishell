/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 23:32:36 by azorlutu          #+#    #+#             */
/*   Updated: 2025/06/30 23:32:36 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_home_path(t_env *env_list)
{
	return (get_env_value(env_list, "HOME"));
}

char	*get_oldpwd_path(t_env *env_list)
{
	return (get_env_value(env_list, "OLDPWD"));
}

void	print_cd_error(char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

int	cd_too_many_args(void)
{
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	return (1);
}

char	*get_cd_target(char *arg, t_minishell *minishell)
{
	char	*oldpwd;

	if (!arg || !ft_strcmp(arg, "~"))
		return (get_home_path(minishell->env_list));
	else if (!ft_strcmp(arg, "-"))
	{
		oldpwd = get_oldpwd_path(minishell->env_list);
		if (!oldpwd)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		ft_putendl_fd(oldpwd, 1);
		return (oldpwd);
	}
	return (arg);
}
