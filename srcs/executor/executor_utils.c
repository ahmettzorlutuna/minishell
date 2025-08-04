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

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	*ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
	char	*first;
	char	*full;

	if (!*s1 || !*s2 || !*s3)
		return (NULL);
	first = ft_strjoin(s1, s2);
	if (!first)
		return (NULL);
	full = ft_strjoin(first, s3);
	free(first);
	return (full);
}

static void	handle_stat_errors(t_minishell *minishell, char *path)
{
	if (errno == ENOENT)
		print_and_exit_free(minishell, path,
			"No such file or directory", 127);
	if (errno == EACCES)
		print_and_exit_free(minishell, path,
			"Permission denied", 126);
	if (errno == ENOTDIR)
		print_and_exit_free(minishell, path,
			"Not a directory", 126);
}

static char	*get_executable_path(t_command *cmd,
						t_minishell *minishell, struct stat *sb)
{
	char	*path;

	path = resolve_path(cmd->args[0], minishell->env_list);
	if (!path || path[0] == '\0')
	{
		if (!cmd->args[0])
			cmd->args[0] = ft_strdup("$");
		print_and_exit(minishell, cmd->args[0],
			"command not found", 127);
	}
	if (stat(path, sb) != 0)
		handle_stat_errors(minishell, path);
	if (S_ISDIR(sb->st_mode))
		print_and_exit_free(minishell, path, "Is a directory", 126);
	if (access(path, X_OK) != 0)
		print_and_exit_free(minishell, path, "Permission denied", 126);
	return (path);
}

void	check_and_execute(t_command *cmd, t_minishell *minishell)
{
	char		*path;
	struct stat	sb;
	int			status;

	if (!cmd->args[0] || cmd->args[0][0] == '\0')
		exit(0);
	if (is_builtin(cmd->args[0]))
	{
		status = run_builtin(cmd, minishell);
		free_minishell(minishell);
		exit(status);
	}
	path = get_executable_path(cmd, minishell, &sb);
	execve(path, cmd->args, minishell->env_array);
	free(path);
	perror("minishell");
	if (errno == EACCES)
	{
		free_minishell(minishell);
		exit(126);
	}
	free_minishell(minishell);
	exit(127);
}
