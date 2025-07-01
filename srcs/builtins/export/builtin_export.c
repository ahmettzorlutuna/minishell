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
			free(key);
			free(value);
			return (print_export_error(args[i]));
		}
		set_env_and_update(minishell, key, value);
		free(key);
		free(value);
		i++;
	}
	return (0);
}
