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

int	builtin_cd(char **args, t_minishell *minishell)
{
	char	*target_path;
	char	cwd[PATH_MAX];

	if (args[1] && args[2])
		return (cd_too_many_args());
	target_path = get_cd_target(args[1], minishell);
	if (!target_path || chdir(target_path) != 0)
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
