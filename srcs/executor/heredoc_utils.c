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

static	char	*heredoc_line_process(t_minishell *minishell,
				t_redirection *redir, char *line)
{
	char	*expanded;

	if (redir->redir_quote == NO_QUOTE)
	{
		expanded = expand_word(minishell,
				NO_QUOTE, line, minishell->env_list);
		free(line);
		line = expanded;
	}
	return (line);
}

char	*heredoc_read_loop(t_minishell *minishell, t_redirection *redir)
{
	char	*line;
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
	{
		free_minishell(minishell);
		exit(1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			free(buffer);
			if (g_signal_flag == SIGINT)
			{
				free_minishell(minishell);
				exit(130);
			}
			free_minishell(minishell);
			exit(0);
		}
		if (ft_strcmp(line, redir->delimiter_raw) == 0)
			break ;
		line = heredoc_line_process(minishell, redir, line);
		buffer = ft_strjoin_free(buffer, line);
		buffer = ft_strjoin_free(buffer, "\n");
		free(line);
	}
	return (buffer);
}

int	run_heredoc_child(t_minishell *minishell,
			t_redirection *redir, int write_fd)
{
	char	*buffer;

	g_signal_flag = 0;
	buffer = heredoc_read_loop(minishell, redir);
	write(write_fd, buffer, ft_strlen(buffer));
	free(buffer);
	close(write_fd);
	free_minishell(minishell);
	exit(0);
}

void	handle_heredoc_child(t_minishell *minishell,
				t_redirection *redir, int *pipefd)
{
	setup_heredoc_signals();
	run_heredoc_child(minishell, redir, pipefd[1]);
}
