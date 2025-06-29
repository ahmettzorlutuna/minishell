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

int run_heredoc_child(const char *delimiter, int write_fd)
{
	char *line;
	char *buffer;
	g_signal_flag = 0;

	buffer = ft_strdup("");
	if(!buffer)
		exit(1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			free(buffer);
			exit(130);
		}
		if (!delimiter || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		buffer = ft_strjoin_free(buffer, line);
		buffer = ft_strjoin_free(buffer, "\n");
	}
	write(write_fd, buffer, ft_strlen(buffer));
	free(buffer);
	close(write_fd);
	exit(0);
}

int handle_heredoc(t_command *cmd, t_minishell *minishell)
{
	t_redirection *redir;
	int pipefd[2];
	pid_t pid;
	int status;

	(void)minishell;
	while (cmd)
	{
		redir = cmd->redirects;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				if (pipe(pipefd) == -1)
					return (1);
				pid = fork();
				if (pid < 0)
					return (1);
				if (pid == 0)
				{
					setup_heredoc_signals();
					run_heredoc_child(redir->filename, pipefd[1]);
				}
				close(pipefd[1]);
				waitpid(pid, &status, 0);
				if(WIFEXITED(status))
				{
					minishell->last_exit_code = WEXITSTATUS(status);
					if(minishell->last_exit_code == 130)
					{
						close(pipefd[0]);
						setup_interactive_signals();
						return (1);
					}
				}
				else if(WIFSIGNALED(status))
				{
					minishell->last_exit_code = 128 + WTERMSIG(status);
				}
				redir->fd = pipefd[0];
			}
			redir = redir->next;
		}
		cmd = cmd->next_pipe;
	}
	setup_interactive_signals();
	return (0);
}
