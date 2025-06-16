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

	g_signal_flag = 0; // Reset signal flag for heredoc
    while (cmd)
    {
        redir = cmd->redirects;
        while (redir)
        {
            if(redir->type == TOKEN_HEREDOC)
            {
                if(pipe(pipefd) == -1)
                    return (1);
				setup_heredoc_signals();
                while (1)
                {
                    line = readline("> ");
					if (!line)
					{
						if (g_signal_flag == SIGINT)
						{
							close(pipefd[0]);
							close(pipefd[1]);
							setup_interactive_signals();
							return (1); // heredoc iptal
						}
						break;
					}
                    if(!line || !redir->filename || ft_strcmp(line, redir->filename) == 0)
                        break;
                    write(pipefd[1], line, ft_strlen(line));
                    write(pipefd[1], "\n", 1);
                    free(line);
                }
				setup_interactive_signals();
                if(line)
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