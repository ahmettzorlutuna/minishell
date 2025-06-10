/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:34:28 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/09 16:34:29 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	builtin_unset(char **args, t_minishell *minishell)
{
	int	i;

	if (!args || !args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (is_valid_key(args[i]))
		{
			unset_env_value(&minishell->env_list, args[i]);
			update_env_array(minishell);
		}
		i++;
	}
	return (0);
}
