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

int	is_builtin(char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strcmp(cmd, "echo")
        || !ft_strcmp(cmd, "cd")
        || !ft_strcmp(cmd, "pwd")
        || !ft_strcmp(cmd, "export")
        || !ft_strcmp(cmd, "unset")
        || !ft_strcmp(cmd, "env")
        || !ft_strcmp(cmd, "exit"));
}

int	run_builtin(t_command *cmd, t_minishell *minishell)
{
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
        return (builtin_env(minishell));
    else if (!ft_strcmp(cmd->args[0], "exit"))
        return (builtin_exit(cmd->args, minishell));
    return (1);
}
