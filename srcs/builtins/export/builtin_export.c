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

int	builtin_export(char **args, t_minishell *minishell)
{
	int		i;
	char	*key;
	char	*value;

	if (!args[1])
		return (handle_no_args(minishell));
	i = 1;
	while (args[i])
	{
		key = NULL;
		value = NULL;
		if (split_key_value(args[i], &key, &value) || !is_valid_key(key))
		{
			print_export_error(args[i]);
			free(key);
			free(value);
			i++;
			continue ;
		}
		set_env_and_update(minishell, key, value);
		free(key);
		free(value);
		i++;
	}
	return (0);
}
