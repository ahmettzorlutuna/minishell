/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 20:31:40 by azorlutu          #+#    #+#             */
/*   Updated: 2025/06/03 20:31:41 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int pipe_safe(int pipe_fd[2])
{
	if(pipe(pipe_fd) == -1)
	{
		perror("pipe error");
		return (1);
	}
	return (0);
}

pid_t fork_safe(void)
{
	pid_t pid;

	pid = fork();
	if(pid == -1)
		perror("fork error");
	return (pid);
}