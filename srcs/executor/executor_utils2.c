/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:31:40 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/26 14:51:05 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	empty_or_null_command(t_command *cmd, t_minishell *minishell)
{
	if (!cmd || !cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
	{
		if (is_quoted_empty_command_error(cmd, minishell))
			return (1);
		if (handle_redirect_only_command(cmd, minishell))
			return (1);
		if (is_only_operator_syntax_error(minishell))
			return (1);
		minishell->last_exit_code = 0;
		return (1);
	}
	return (0);
}

int	run_parent_builtin_if_needed(t_command *cmd, t_minishell *minishell)
{
	int	in_bak;
	int	out_bak;
	int	redir;

	if (!cmd->next_pipe && is_builtin(cmd->args[0]))
	{
		in_bak = dup(STDIN_FILENO);
		out_bak = dup(STDOUT_FILENO);
		if (in_bak == -1 || out_bak == -1)
		{
			perror("dup");
			exit(1);
		}
		redir = set_redirection_fds(cmd->redirects);
		if (redir != 0)
		{
			minishell->last_exit_code = 1;
			return (restore_fds_and_return(in_bak, out_bak, 1, minishell));
		}
		minishell->last_exit_code = run_builtin(cmd, minishell);
		return (restore_fds_and_return(in_bak, out_bak, 1, minishell));
	}
	return (0);
}

void	execute_pipeline_fork(t_command *cmd, t_minishell *minishell)
{
	int		prev_fd;
	pid_t	pid;
	int		status;

	prev_fd = -1;
	status = 0;
	if (handle_heredoc(cmd, minishell) != 0)
		return ;
	while (cmd)
	{
		pid = process_pipeline_command(cmd, minishell, &prev_fd);
		if (pid == -1)
			return ;
		cmd = cmd->next_pipe;
	}
	waitpid(pid, &status, 0);
	finalize_pipeline_status(status, minishell);
}

int	pipe_safe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		return (1);
	}
	return (0);
}

pid_t	fork_safe(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror("fork error");
	return (pid);
}
