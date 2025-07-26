/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 21:04:43 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/26 21:04:44 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	setup_signals_and_fork(int pipe_fd[2], pid_t *pid, t_command *cmd)
{
	if (cmd->next_pipe && pipe_safe(pipe_fd))
		return (-1);
	*pid = fork_safe();
	if (*pid == -1)
		return (-1);
	return (0);
}

static void	prepare_child_execution(t_command *cmd,
	t_minishell *minishell, int prev_fd, int pipe_fd[2])
{
	setup_default_signals();
	if (set_redirection_fds(cmd->redirects) != 0)
	{
		free_minishell(minishell);
		exit(1);
	}
	setup_child_processes(cmd, prev_fd, pipe_fd, minishell);
}

int	process_pipeline_command(t_command *cmd,
	t_minishell *minishell, int *prev_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (setup_signals_and_fork(pipe_fd, &pid, cmd) != 0)
		return (-1);
	if (pid == 0)
		prepare_child_execution(cmd, minishell, *prev_fd, pipe_fd);
	else
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, sigquit_handler);
	}
	handle_parent_process(*prev_fd, pipe_fd, cmd);
	if (cmd->next_pipe)
		*prev_fd = pipe_fd[0];
	return (pid);
}
