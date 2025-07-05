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
	long	exit_code;

	ft_putstr_fd("exit\n", 1);
	if (!args[1])
	{
		free_minishell(minishell);
		exit(minishell->last_exit_code);
	}
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_minishell(minishell);
		exit(2);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		minishell->last_exit_code = 1;
		free_minishell(minishell);
		exit(1);
	}
	exit_code = ft_atol(args[1]) % 256;
	if (exit_code < 0)
		exit_code += 256;
	free_minishell(minishell);
	exit(exit_code);
}
