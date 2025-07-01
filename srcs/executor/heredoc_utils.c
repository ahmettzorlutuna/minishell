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

char	*heredoc_read_loop(const char *delimiter)
{
	char	*line;
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
		exit(1);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			free(buffer);
			if (g_signal_flag == SIGINT)
				exit(130);
			exit(0);
		}
		if (!delimiter || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		buffer = ft_strjoin_free(buffer, line);
		buffer = ft_strjoin_free(buffer, "\n");
	}
	return (buffer);
}

int	run_heredoc_child(const char *delimiter, int write_fd)
{
	char	*buffer;

	g_signal_flag = 0;
	buffer = heredoc_read_loop(delimiter);
	write(write_fd, buffer, ft_strlen(buffer));
	free(buffer);
	close(write_fd);
	exit(0);
}

void	handle_heredoc_child(t_redirection *redir, int *pipefd)
{
	setup_heredoc_signals();
	run_heredoc_child(redir->filename, pipefd[1]);
}
