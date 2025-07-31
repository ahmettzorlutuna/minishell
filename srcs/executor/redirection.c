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

static int	open_redirect_fd(t_redirection *redir, int *fd_out)
{
	if (redir->type == TOKEN_REDIRECT_IN)
		*fd_out = open(redir->filename, O_RDONLY);
	else if (redir->type == TOKEN_REDIRECT_OUT)
		*fd_out = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_APPEND)
		*fd_out = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == TOKEN_HEREDOC)
		*fd_out = redir->fd;
	else
	{
		ft_putstr_fd("syntax error: unexpected redirection\n", 2);
		return (1);
	}
	return (0);
}

int	create_empty_redirect_files(t_redirection *redir)
{
	int	fd;

	while (redir)
	{
		if (open_redirect_fd(redir, &fd) != 0)
			return (1);
		if (fd < 0)
		{
			perror(redir->filename);
			return (1);
		}
		close(fd);
		redir = redir->next;
	}
	return (0);
}

static int	open_redirection(t_redirection *redir)
{
	int	fd;

	if (redir->type == TOKEN_REDIRECT_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == TOKEN_REDIRECT_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == TOKEN_HEREDOC)
		fd = redir->fd;
	else
		return (-2);
	return (fd);
}

static void	redirect_fd(t_redirection *redir, int fd)
{
	if (redir->type == TOKEN_REDIRECT_IN || redir->type == TOKEN_HEREDOC)
	{
		dup2(fd, STDIN_FILENO);
		if (redir->type != TOKEN_HEREDOC)
			close(fd);
	}
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

int	set_redirection_fds(t_redirection *redir)
{
	int	fd;

	while (redir)
	{
		fd = open_redirection(redir);
		if (fd == -2)
			return (1);
		if (fd < 0)
		{
			perror(redir->filename);
			return (1);
		}
		redirect_fd(redir, fd);
		redir = redir->next;
	}
	return (0);
}
