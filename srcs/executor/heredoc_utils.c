/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 00:31:27 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/01 00:31:28 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	run_heredoc_child(t_minishell *minishell,
			t_redirection *redir, int write_fd)
{
	char	*buffer;

	g_signal_flag = 0;
	buffer = heredoc_read_loop(minishell, redir);
	write(write_fd, buffer, ft_strlen(buffer));
	free(buffer);
	close(write_fd);
	free_minishell(minishell);
	exit(0);
}

void	handle_heredoc_child(t_minishell *minishell,
				t_redirection *redir, int *pipefd)
{
	setup_heredoc_signals();
	run_heredoc_child(minishell, redir, pipefd[1]);
}
