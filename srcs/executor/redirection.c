/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 23:09:38 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/30 23:09:39 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int set_redirection_fds(t_redirection *redir)
{
	int fd;

	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == TOKEN_REDIRECT_OUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == TOKEN_APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == TOKEN_HEREDOC)
			fd = redir->fd;
		else
			return (1);
		if (fd < 0)
		{
			perror(redir->filename);
			return (1);
		}
		if (redir->type == TOKEN_REDIRECT_IN || redir->type == TOKEN_HEREDOC)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		if (redir->type != TOKEN_HEREDOC)
			close(fd);
		redir = redir->next;
	}
	return (0);
}