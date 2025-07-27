/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 12:15:10 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/27 12:15:10 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	char	*init_heredoc_buffer(t_minishell *minishell)
{
	char	*buffer;

	buffer = ft_strdup("");
	if (!buffer)
	{
		free_minishell(minishell);
		exit(1);
	}
	return (buffer);
}

static	char	*handle_heredoc_eof(t_minishell *minishell, char *buffer)
{
	if (g_signal_flag == SIGINT)
	{
		free(buffer);
		free_minishell(minishell);
		exit(130);
	}
	return (buffer);
}

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

static	char	*append_heredoc_line(t_minishell *minishell,
								t_redirection *redir, char *line, char *buffer)
{
	line = heredoc_line_process(minishell, redir, line);
	buffer = ft_strjoin_free(buffer, line);
	buffer = ft_strjoin_free(buffer, "\n");
	free(line);
	return (buffer);
}

char	*heredoc_read_loop(t_minishell *minishell, t_redirection *redir)
{
	char	*line;
	char	*buffer;

	buffer = init_heredoc_buffer(minishell);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			handle_heredoc_eof(minishell, buffer);
			break ;
		}
		if (ft_strcmp(line, redir->delimiter_raw) == 0)
			break ;
		buffer = append_heredoc_line(minishell, redir, line, buffer);
	}
	return (buffer);
}
