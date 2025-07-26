/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 23:42:04 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/26 23:42:04 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	setup_child_processes(t_command *cmd,
					int prev_fd, int pipe_fd[2], t_minishell *minishell)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next_pipe)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (set_redirection_fds(cmd->redirects) != 0)
	{
		free_minishell(minishell);
		exit(minishell->last_exit_code);
	}
	check_and_execute(cmd, minishell);
}

int	handle_parent_process(int prev_fd, int pipe_fd[2], t_command *cmd)
{
	int	status;

	status = 0;
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next_pipe)
	{
		close(pipe_fd[1]);
		prev_fd = pipe_fd[0];
	}
	return (status);
}

void	finalize_pipeline_status(int status, t_minishell *minishell)
{
	setup_interactive_signals();
	if (WIFEXITED(status))
		minishell->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		minishell->last_exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
	else
		minishell->last_exit_code = 1;
}

void	execute_pipeline(t_command *cmd, t_minishell *minishell)
{
	if (minishell->has_syntax_error)
		return ;
	if (empty_or_null_command(cmd, minishell))
		return ;
	if (run_parent_builtin_if_needed(cmd, minishell))
		return ;
	execute_pipeline_fork(cmd, minishell);
}
