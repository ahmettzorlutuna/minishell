/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 17:07:10 by azorlutu          #+#    #+#             */
/*   Updated: 2025/06/22 17:07:11 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void free_redirections(t_redirection *redir)
{
    t_redirection *tmp;

    while (redir)
    {
        tmp = redir->next;
        if(redir->filename)
            free(redir->filename);
        free(redir);
        redir = tmp;
    }
}

static void free_command_list(t_command *cmd)
{
    t_command *tmp;
    int i;

    while(cmd)
    {
        tmp = cmd->next_pipe;
        if(cmd->args)
        {
            i = 0;
            while (cmd->args[i])
                free(cmd->args[i++]);
            free(cmd->args);
        }
        if(cmd->redirects)
            free_redirections(cmd->redirects);
        free(cmd);
        cmd = tmp;
    }
}

void free_loop(t_minishell *minishell)
{
    free(minishell->input);
    free_token_list(minishell->tokens);
    free_command_list(minishell->command_list);
    minishell->input = NULL;
    minishell->tokens = NULL;
    minishell->command_list = NULL;
}

void free_minishell(t_minishell *minishell)
{
    if(minishell->env_array)
	    free_env_array(minishell->env_array);
    if(minishell->env_list)
        free_env_list(minishell->env_list);
    free(minishell);
}
