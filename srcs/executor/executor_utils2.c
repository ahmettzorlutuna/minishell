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

int	run_pipeline_commands(t_command *cmd, t_minishell *minishell,
							pid_t *pids)
{
	int	pid;
	int	pid_count;
	int	prev_fd;

	pid_count = 0;
	prev_fd = -1;
	if (handle_heredoc(cmd, minishell) != 0)
		return (-1);
	while (cmd)
	{
		pid = process_pipeline_command(cmd, minishell, &prev_fd);
		if (pid == -1)
			return (-1);
		pids[pid_count++] = pid;
		cmd = cmd->next_pipe;
	}
	return (pid_count);
}

void	execute_pipeline_fork(t_command *cmd, t_minishell *minishell)
{
	pid_t	*pids;
	int		pipe_count;
	int		pid_count;

	pipe_count = get_pipe_count(cmd);
	pids = malloc(sizeof(pid_t) * pipe_count);
	minishell->temp_pids = pids;
	if (!pids)
	{
		perror("malloc");
		exit(1);
	}
	pid_count = run_pipeline_commands(cmd, minishell, pids);
	if (pid_count >= 0)
		wait_all_children(pids, pid_count, minishell);
	free(minishell->temp_pids);
	minishell->temp_pids = NULL;
}
