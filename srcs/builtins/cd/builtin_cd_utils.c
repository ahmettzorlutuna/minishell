/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 23:32:36 by azorlutu          #+#    #+#             */
/*   Updated: 2025/08/01 21:42:04 by ekamar           ###   ########.fr       */
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
