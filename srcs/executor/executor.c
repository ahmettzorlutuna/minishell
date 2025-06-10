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

static void setup_child_processes(t_command *cmd, int prev_fd, int pipe_fd[2], t_minishell *minishell)
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
		exit(minishell->last_exit_code);
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, minishell));
	execve(resolve_path(cmd->args[0], minishell->env_list), cmd->args, minishell->env_array);
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd("\n", 2);
	exit(127);
}

static int handle_parent_process(pid_t pid, int prev_fd, int pipe_fd[2], t_command *cmd)
{
	int status;

	status = 0;
	if (prev_fd != -1)
		close(prev_fd);
	if (cmd->next_pipe)
	{
		close(pipe_fd[1]);
		prev_fd = pipe_fd[0];
	}
	waitpid(pid, &status, 0);
	return (status);
}

static void execute_pipeline_fork(t_command *cmd, t_minishell *minishell)
{
	int pipe_fd[2];
	int prev_fd = -1;
	pid_t pid;
	int status;

	status = 0;
	if (handle_heredoc(cmd) != 0)
	{
		minishell->last_exit_code = 1;
		return;
	}
	while (cmd)
	{
		if (cmd->next_pipe && pipe_safe(pipe_fd))
			return;
		pid = fork_safe();
		if (pid == -1)
			return;
		else if (pid == 0)
		{
			if (set_redirection_fds(cmd->redirects) != 0)
				exit(1);
			setup_child_processes(cmd, prev_fd, pipe_fd, minishell);
		}
		status = handle_parent_process(pid, prev_fd, pipe_fd, cmd);
		if (cmd->next_pipe)
			prev_fd = pipe_fd[0];
		cmd = cmd->next_pipe;
	}
	if (WIFEXITED(status))
		minishell->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		minishell->last_exit_code = 128 + WTERMSIG(status);
	else
		minishell->last_exit_code = 1;
}

void execute_pipeline(t_command *cmd, t_minishell *minishell)
{
	if (!cmd)
		return;
	if (!cmd->next_pipe && is_parent_builtin(cmd->args[0]))
	{
		if (handle_heredoc(cmd) != 0)
		{
			minishell->last_exit_code = 1;
			return;
		}
		if (set_redirection_fds(cmd->redirects) != 0)
		{
			minishell->last_exit_code = 1;
			return;
		}
		minishell->last_exit_code = run_builtin(cmd, minishell);
		return;
	}
	else
	{
		execute_pipeline_fork(cmd, minishell);
	}
}