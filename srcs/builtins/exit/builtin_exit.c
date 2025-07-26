/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:33:48 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/09 16:33:50 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args, t_minishell *minishell)
{
	ft_putstr_fd("exit\n", 1);
	if (!args[1])
		no_args_exit(minishell);
	if (!is_numeric(args[1]))
		non_numeric_exit(args[1], minishell);
	if (args[2])
		too_many_args_exit(minishell);
	normal_exit(args[1], minishell);
	return (0);
}
