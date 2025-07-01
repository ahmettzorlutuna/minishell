/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 01:02:15 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/31 01:02:15 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	process_heredoc_result(t_minishell *minishell, int status, int fd)
{
	if (WIFEXITED(status))
	{
		minishell->last_exit_code = WEXITSTATUS(status);
		if (minishell->last_exit_code == 130)
		{
			close(fd);
			setup_interactive_signals();
			return (1);
		}
	}
	else if (WIFSIGNALED(status))
		minishell->last_exit_code = 128 + WTERMSIG(status);
	return (0);
}

static int	process_heredoc_pipe(t_redirection *redir, t_minishell *minishell)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (1);
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
		handle_heredoc_child(redir, pipefd);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if (process_heredoc_result(minishell, status, pipefd[0]))
		return (1);
	redir->fd = pipefd[0];
	return (0);
}

int	handle_heredoc(t_command *cmd, t_minishell *minishell)
{
	t_redirection	*redir;

	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				if (process_heredoc_pipe(redir, minishell))
					return (1);
			}
			redir = redir->next;
		}
		cmd = cmd->next_pipe;
	}
	setup_interactive_signals();
	return (0);
}
