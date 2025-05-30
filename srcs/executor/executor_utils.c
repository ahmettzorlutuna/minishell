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
    char	**paths;
    char	*path_value;
    char	*temp_path;
    int		i;

	if(!cmd || !*cmd)
		return (NULL);
	if(ft_strchr(cmd, '/') && is_path_executable(cmd))
		return (ft_strdup(cmd));

	path_value = get_env_value(env_list, "PATH");
	if(!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if(!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp_path = ft_strjoin_three(paths[i], "/", cmd);
		if(is_path_executable(temp_path))
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

    if(!split)
        return ;
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

    if(!*s1 || !*s2 || !*s3)
        return (NULL);

    first = ft_strjoin(s1, s2);
    if(!first)
        return (NULL);
    full = ft_strjoin(first, s3);
    free(first);
    return (full);
}

int is_path_executable(const char *path)
{
	return (access(path, X_OK) == 0);
}

