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
	setup_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal_flag == SIGINT) //ctrl + D
			{
				free(buffer);
				exit(1);
			}
			break;
		}
		if (!delimiter || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		buffer = ft_strjoin(buffer, line);
		buffer = ft_strjoin(buffer, "\n");
		free(line);
	}

	if(g_signal_flag != SIGINT)
		write(write_fd, buffer,	ft_strlen(buffer));

	free(buffer);
	close(write_fd);
	exit(0);
}

int handle_heredoc(t_command *cmd)
{
	t_redirection *redir;
	int pipefd[2];
	pid_t pid;
	int status;

	g_signal_flag = 0;
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
					run_heredoc_child(redir->filename, pipefd[1]);
				close(pipefd[1]);
				waitpid(pid, &status, 0);
				if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
				{
					write(1, "\n", 1);
					close(pipefd[0]);
					setup_interactive_signals();
					return (1);
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
