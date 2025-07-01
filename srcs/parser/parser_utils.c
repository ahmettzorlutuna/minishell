/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:45:20 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/01 20:45:21 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "../includes/minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
	{
		perror("Error allocating memory for command");
		return (NULL);
	}
	return (cmd);
}

void	handle_pipe_recursively(t_token **cursor, t_command *cmd)
{
	if (*cursor && (*cursor)->type == TOKEN_PIPE)
	{
		*cursor = (*cursor)->next;
		cmd->next_pipe = parse_command(cursor);
	}
}
