/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:10:57 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/28 00:10:57 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *resolve_path(const char *cmd, t_env *env_list)
{
	char **paths;
	char *path_value;
	char *temp_path;
	int i;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));

	path_value = get_env_value(env_list, "PATH");
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp_path = ft_strjoin_three(paths[i], "/", cmd);
		if (access(temp_path, F_OK) == 0)
		{
			free_split(paths);
			return (temp_path);
		}
		free(temp_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}

void free_split(char **split)
{
	int i;

	if (!split)
		return;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char *ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char *first;
	char *full;

	if (!*s1 || !*s2 || !*s3)
		return (NULL);
	first = ft_strjoin(s1, s2);
	if (!first)
		return (NULL);
	full = ft_strjoin(first, s3);
	free(first);
	return (full);
}

void print_and_exit(char *prefix, char *msg, int code)
{
	ft_putstr_fd("minishell: ", 2);
	if (prefix)
	{
		ft_putstr_fd(prefix, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
	exit(code);
}

void check_and_execute(t_command *cmd, t_minishell *minishell)
{
	char *path;
	struct stat sb;

	if (!cmd->args[0] || cmd->args[0][0] == '\0')
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(run_builtin(cmd, minishell));
	path = resolve_path(cmd->args[0], minishell->env_list);
	if (!path || path[0] == '\0')
	{
		if(!cmd->args[0])
		{
			free(cmd->args[0]);
			cmd->args[0] = ft_strdup("$");
		}
		print_and_exit(cmd->args[0], "command not found", 127);
	}
	if (stat(path, &sb) != 0)
	{
		if (errno == ENOENT)
			print_and_exit(path, "No such file or directory", 127);
		if (errno == EACCES)
			print_and_exit(path, "Permission denied", 126);
	}
	if (S_ISDIR(sb.st_mode))
		print_and_exit(path, "Is a directory", 126);
	if (access(path, X_OK) != 0)
		print_and_exit(path, "Permission denied", 126);
	setup_default_signals();
	execve(path, cmd->args, minishell->env_array);
	perror("minishell");
	if (errno == EACCES)
		exit(126);
	else
		exit(127);
}
