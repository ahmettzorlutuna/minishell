/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:35:49 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/26 14:35:50 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	no_args_exit(t_minishell *minishell)
{
	int	exit_code;

	exit_code = minishell->last_exit_code;
	free_minishell(minishell);
	exit(exit_code);
}

void	non_numeric_exit(char *arg, t_minishell *minishell)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	free_minishell(minishell);
	exit(2);
}

int	too_many_args_exit(void)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	return (1);
}

void	normal_exit(char *arg, t_minishell *minishell)
{
	long	exit_code;

	exit_code = ft_atol(arg) % 256;
	if (exit_code < 0)
		exit_code += 256;
	free_minishell(minishell);
	exit(exit_code);
}
