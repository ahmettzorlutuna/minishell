/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:49:57 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/04 17:49:58 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t	g_signal_flag = 0;

static void shell_loop(t_minishell *minishell)
{
	while (1)
	{
		minishell->input = readline("minishell> ");
		if (minishell->input == NULL)
		{
			ft_putstr_fd("exit\n", 0);
			exit_minishell(minishell);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*minishell;

	(void)argv;

	init_signal_handler();
	minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!minishell)
		return (ft_putstr_fd("Error: Memory allocation failed\n",
				2), EXIT_FAILURE);
	if (argc != 1)
	{
		perror("No arguments expected for minishell");
		free(minishell);
		exit(2);
	}
	init_minishell(minishell,envp);
	set_env_value(&minishell->env_list, "a3", "3");
	shell_loop(minishell);
	exit_minishell(minishell);
	return (0);
}
