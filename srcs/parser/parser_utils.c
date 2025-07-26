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

void	handle_pipe_recursively(t_minishell *minishell,
				t_token **cursor, t_command *cmd)
{
	if (*cursor && (*cursor)->type == TOKEN_PIPE)
	{
		*cursor = (*cursor)->next;
		if (!*cursor || (*cursor)->type == TOKEN_PIPE)
		{
			ft_putstr_fd("minishell: syntax error", 2);
			ft_putstr_fd(" near unexpected token `|'\n", 2);
			minishell->last_exit_code = 2;
			minishell->has_syntax_error = 1;
			return ;
		}
		cmd->next_pipe = parse_command(minishell, cursor);
		if (!cmd->next_pipe)
		{
			free_command_list(cmd);
			cmd = NULL;
			return ;
		}
	}
}
