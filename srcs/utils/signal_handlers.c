/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:49:00 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/26 20:49:01 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	g_signal_flag = SIGINT;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_heredoc_handler(int sig)
{
	(void)sig;
	g_signal_flag = SIGINT;
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	close(0);
}

void	sigquit_handler(int sig)
{
	(void)sig;
	g_signal_flag = SIGINT;
	rl_replace_line("", 0);
	write(1, "Quit (core dumped)\n", 19);
	rl_on_new_line();
}
