/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 23:42:04 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/26 23:42:04 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int is_path_executable(const char *path)
{
	return (access(path, X_OK) == 0);
}

static	char *resolve_path(const char *cmd, t_env *env_list)
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

void execute_single_command(t_command *cmd, t_minishell *minishell)
{
	pid_t	pid;
	int		status;
	char	*path;

	status = 0;
	if(!cmd || !cmd->args || !cmd->args[0])
		return;
	path = resolve_path(cmd->args[0], minishell->env_list);
	if(!path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n",2);
		minishell->last_exit_code = 127;
		return ;
	}

	pid = fork();
	if(pid == 0)
	{
		execve(path, cmd->args, minishell->env_array);
		perror("execve");
		exit(126);
	}
	else if(pid > 0)
		waitpid(pid, &status, 0);
	else
		perror("fork");
	
	free(path);
	if(WIFEXITED(status))
		minishell->last_exit_code = WIFEXITED(status);
}