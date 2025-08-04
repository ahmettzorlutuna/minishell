/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_null_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:41:25 by azorlutu          #+#    #+#             */
/*   Updated: 2025/08/04 22:41:26 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_null_command_case(t_command *cmd, t_minishell *minishell)
{
	if (!cmd || !minishell->tokens)
	{
		if (minishell->tokens)
			minishell->last_exit_code = 2;
		else
			minishell->last_exit_code = 0;
		if (!cmd)
		{
			ft_putstr_fd("minishell: syntax error", 2);
			ft_putstr_fd(" near unexpected token `newline'\n", 2);
		}
		return (1);
	}
	return (0);
}

static int	handle_empty_args_case(t_command *cmd, t_minishell *minishell)
{
	if ((!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		&& (ft_strcmp(minishell->tokens->value, "<<") != 0))
	{
		if (is_quoted_empty_command_error(cmd, minishell))
			return (1);
		if (handle_redirect_only_command(cmd, minishell))
			return (1);
		minishell->last_exit_code = 0;
		return (1);
	}
	return (0);
}

int	empty_or_null_command(t_command *cmd, t_minishell *minishell)
{
	if (is_only_operator_syntax_error(minishell))
		return (1);
	if (handle_null_command_case(cmd, minishell))
		return (1);
	if (handle_empty_args_case(cmd, minishell))
		return (1);
	return (0);
}
