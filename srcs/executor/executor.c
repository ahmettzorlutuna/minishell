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
		minishell->last_exit_code = WEXITSTATUS(status);
}

void execute_pipeline(t_command *cmd, t_minishell *minishell)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;
	char *path1;
	char *path2;

	if(!cmd || !cmd->next_pipe)
		return;
	if(pipe(fd) == -1)
	{
		perror("pipe error");
		return;
	}

	pid1 = fork();
	if(pid1 == 0)
	{
		// Child 1 → stdout'u pipe'a ver
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]); // Read ucunu kapat
		close(fd[1]);

		path1 = resolve_path(cmd->args[0], minishell->env_list);
		if (!path1)
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		execve(path1, cmd->args, minishell->env_array);
		perror("execve cmd1");
		exit(126);
	}

	pid2 = fork();
	if(pid2 == 0)
	{
		// Child 2 → stdin'i pipe'tan al
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]); // Write ucunu kapat
		close(fd[0]);

		path2 = resolve_path(cmd->next_pipe->args[0], minishell->env_list);
		if (!path2)
		{
			ft_putstr_fd(cmd->next_pipe->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			exit(127);
		}
		execve(path2, cmd->next_pipe->args, minishell->env_array);
		perror("execve cmd2");
		exit(126);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if(WIFEXITED(status))
			minishell->last_exit_code = WEXITSTATUS(status);
}

void execute_command_list(t_command *cmd, t_minishell *minishell)
{
	if(!cmd)
		return ;
	if(!cmd->next_pipe)
		execute_single_command(cmd, minishell);
	else
		execute_pipeline(cmd, minishell);
}