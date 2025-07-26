/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 14:46:21 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/26 14:50:44 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_quoted_empty_command_error(t_command *cmd, t_minishell *minishell)
{
	if (cmd && cmd->is_quoted_empty_command)
	{
		ft_putstr_fd("minishell: '' : command not found\n", 2);
		minishell->last_exit_code = 127;
		return (1);
	}
	return (0);
}

int	handle_redirect_only_command(t_command *cmd, t_minishell *minishell)
{
	t_redirection	*redir;

	if (!cmd || !cmd->redirects)
		return (0);
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->type == TOKEN_REDIRECT_IN || redir->type == TOKEN_HEREDOC)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->redirects->filename, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			minishell->last_exit_code = 1;
			return (1);
		}
		redir = redir->next;
	}
	if (create_empty_redirect_files(cmd->redirects) != 0)
	{
		minishell->last_exit_code = 1;
		return (1);
	}
	minishell->last_exit_code = 0;
	return (1);
}

int	is_only_operator_syntax_error(t_minishell *minishell)
{
	char	*value;

	if (!minishell->tokens || !minishell->tokens->value)
		return (0);
	value = minishell->tokens->value;
	if (ft_strcmp(value, "|") == 0 || ft_strcmp(value, "<") == 0
		|| ft_strcmp(value, "<<") == 0 || ft_strcmp(value, ">") == 0
		|| ft_strcmp(value, ">>") == 0)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(value, 2);
		ft_putstr_fd("'\n", 2);
		minishell->last_exit_code = 2;
		return (1);
	}
	return (0);
}
