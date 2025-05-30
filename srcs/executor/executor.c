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

static	int pipe_safe(int pipe_fd[2])
{
	if(pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		return (1);
	}
	return (0);
}

static	pid_t fork_safe(void)
{
	pid_t pid;

	pid = fork();
	if(pid == -1)
		perror("fork error");
	return (pid);
}

static	void setup_child_processes(t_command *cmd, int prev_fd, int	pipe_fd[2],	t_minishell *minishell)
{
	if(prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if(cmd->next_pipe)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1],STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	execve(resolve_path(cmd->args[0],minishell->env_list), cmd->args, minishell->env_array);
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd("\n", 2);
	exit(minishell->last_exit_code);
}

static int handle_parent_process(pid_t pid, int prev_fd, int pipe_fd[2], t_command *cmd)
{
	int status;

	status = 0;
	if(prev_fd != -1)
		close(prev_fd);
	if(cmd->next_pipe)
	{
		close(pipe_fd[1]);
		prev_fd = pipe_fd[0];
	}
	waitpid(pid, &status, 0);
	return (status);
}

void execute_pipeline(t_command *cmd, t_minishell *minishell)
{
	int	pipe_fd[2];
	int prev_fd = -1;
	pid_t pid;
	int status;

	status = 0;
	while (cmd)
	{
		if(cmd->next_pipe && pipe_safe(pipe_fd))
			return ;
		pid = fork_safe();
		if(pid == -1)
			return ;
		else if(pid == 0)
		{
			if(set_redirection_fds(cmd->redirects) != 0)
				exit(minishell->last_exit_code);
			setup_child_processes(cmd, prev_fd, pipe_fd, minishell);
		}
		status = handle_parent_process(pid, prev_fd, pipe_fd, cmd);
		if(cmd->next_pipe)
			prev_fd = pipe_fd[0];
		cmd = cmd->next_pipe;
	}
	if(WIFEXITED(status))
		minishell->last_exit_code = WEXITSTATUS(status);
}