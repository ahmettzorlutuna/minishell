/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 22:32:54 by azorlutu          #+#    #+#             */
/*   Updated: 2025/06/02 22:32:55 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	run_builtin(t_command *cmd, t_minishell *minishell)
{
    (void)minishell;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	else if (!ft_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args, minishell));
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (builtin_export(cmd->args, minishell));
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (builtin_unset(cmd->args, minishell));
	else if (!ft_strcmp(cmd->args[0], "env"))
		return (builtin_env(cmd->args, minishell));
	else if (!ft_strcmp(cmd->args[0], "exit"))
		return (builtin_exit(cmd->args, minishell));
	return (1);
}

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit"));
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "echo"));
}

int execute_single_builtin(t_command *cmd, t_minishell *minishell)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	minishell->last_exit_code = run_builtin(cmd, minishell);
	return (0);
}

int	is_valid_key(const char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}