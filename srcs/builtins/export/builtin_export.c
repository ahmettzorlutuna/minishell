/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekamar <ekamar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:33:57 by azorlutu          #+#    #+#             */
/*   Updated: 2025/08/01 20:44:56 by ekamar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_export_arg(char *arg, t_minishell *minishell)
{
	char	*key;
	char	*value;

	key = NULL;
	value = NULL;
	if (split_key_value(arg, &key, &value) || !is_valid_key(key))
	{
		print_export_error(arg);
		free(key);
		free(value);
		return (1);
	}
	set_env_and_update(minishell, key, value);
	free(key);
	free(value);
	return (0);
}

int	builtin_export(char **args, t_minishell *minishell)
{
	int		i;
	int		error_code;

	error_code = 0;
	if (!args[1])
		return (handle_no_args(minishell));
	i = 1;
	while (args[i])
	{
		if (handle_export_arg(args[i], minishell))
			error_code = 1;
		i++;
	}
	return (error_code);
}
