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

int handle_heredoc(t_command *cmd)
{
    t_redirection *redir;
    int pipefd[2];
    char *line;

    while (cmd)
    {
        redir = cmd->redirects;
        while (redir)
        {
            if(redir->type == TOKEN_HEREDOC)
            {
                if(pipe(pipefd) == -1)
                    return (1);
                while (1)
                {
                    line = readline("> ");
                    if(!line || ft_strcmp(line, redir->filename) == 0)
                        break;
                    write(pipefd[1], line, ft_strlen(line));
                    write(pipefd[1], "\n", 1);
                    free(line);
                }
                free(line);
                close(pipefd[1]);
                redir->fd = pipefd[0];
            }
            redir = redir->next;
        }
        cmd = cmd->next_pipe;
    }
    return (0);
}