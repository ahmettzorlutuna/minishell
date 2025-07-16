/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 23:34:59 by azorlutu          #+#    #+#             */
/*   Updated: 2025/06/30 23:35:00 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	restore_fds_and_return(int in_bak, int out_bak,
				int ret, t_minishell *minishell)
{
	if (dup2(in_bak, STDIN_FILENO) == -1
		|| dup2(out_bak, STDOUT_FILENO) == -1)
	{
		perror("dup2 restore");
		free_minishell(minishell);
		exit(1);
	}
	close(in_bak);
	close(out_bak);
	return (ret);
}

static char	**split_path_list(t_env *env_list)
{
	char	*path_value;

	path_value = get_env_value(env_list, "PATH");
	if (!path_value)
		return (NULL);
	return (ft_split(path_value, ':'));
}

static char	*search_in_paths(char **paths, const char *cmd)
{
	char	*temp_path;
	int		i;

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

char	*resolve_path(const char *cmd, t_env *env_list)
{
	char	**paths;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = split_path_list(env_list);
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
