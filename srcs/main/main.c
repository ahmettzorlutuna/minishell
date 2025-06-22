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
		if(minishell->input && *minishell->input)
		{
			init_minishell(minishell);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	*minishell;
	(void)argv;

	setup_interactive_signals();
	minishell = ft_calloc(1, sizeof(t_minishell));
	if (!minishell)
		return (ft_putstr_fd("Error: Memory allocation failed\n",
				2), EXIT_FAILURE);
	if (argc != 1)
	{
		perror("No arguments expected for minishell");
		free(minishell);
		exit(minishell->last_exit_code);
	}
	init_mini_data(minishell,envp);
	shell_loop(minishell);
	exit_minishell(minishell);
	return (0);
}
