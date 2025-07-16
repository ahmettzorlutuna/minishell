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

void	free_redirections(t_redirection *redir)
{
	t_redirection	*tmp;

	while (redir)
	{
		tmp = redir->next;
		if (redir->filename)
			free(redir->filename);
		if (redir->delimiter_raw)
			free(redir->delimiter_raw);
		if (redir->delimiter_expanded)
			free(redir->delimiter_expanded);
		if (redir->fd != -1)
			close(redir->fd);
		free(redir);
		redir = tmp;
	}
}

void	free_command_list(t_command *cmd)
{
	t_command	*tmp;
	int			i;

	if (!cmd)
		return ;
	while (cmd)
	{
		tmp = cmd->next_pipe;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		if (cmd->redirects)
			free_redirections(cmd->redirects);
		free(cmd);
		cmd = tmp;
	}
}

void	free_arg_list(t_list *args)
{
	t_list	*tmp;

	while (args)
	{
		tmp = args->next;
		if (args->content)
			free(args->content);
		free(args);
		args = tmp;
	}
}

void	free_loop(t_minishell *minishell)
{
	if (minishell->input)
	{
		free(minishell->input);
		minishell->input = NULL;
	}
	if (minishell->tokens)
	{
		free_token_list(minishell->tokens);
		minishell->tokens = NULL;
	}
	if (minishell->command_list)
	{
		free_command_list(minishell->command_list);
		minishell->command_list = NULL;
	}
}

void	free_minishell(t_minishell *minishell)
{
	free_loop(minishell);
	if (minishell->env_array)
		free_env_array(minishell->env_array);
	if (minishell->env_list)
		free_env_list(minishell->env_list);
	rl_clear_history();
	free(minishell);
}
