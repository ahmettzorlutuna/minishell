/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:42:49 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/04 20:42:50 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	parse_redirection(t_token *cursor, t_command *cmd)
{
	t_redirection	*redir;
	t_redirection	**redir_ptr;

	if (!cursor->next || cursor->next->type != TOKEN_WORD)
		return (1);
	redir = ft_calloc(1, sizeof(t_redirection));
	if (!redir)
		return (1);
	redir->fd = -1;
	redir->type = cursor->type;
	redir->filename = ft_strdup(cursor->next->value);
	redir_ptr = &cmd->redirects;
	while (*redir_ptr)
		redir_ptr = &(*redir_ptr)->next;
	*redir_ptr = redir;
	return (0);
}

static int	process_token(t_token **cursor, t_command *cmd, t_list **args)
{
	if ((*cursor)->type == TOKEN_WORD)
		add_arg_to_list(args, (*cursor)->value);
	else if ((*cursor)->type == TOKEN_REDIRECT_IN
		|| (*cursor)->type == TOKEN_REDIRECT_OUT
		|| (*cursor)->type == TOKEN_HEREDOC
		|| (*cursor)->type == TOKEN_APPEND)
	{
		if (parse_redirection(*cursor, cmd))
			return (1);
		*cursor = (*cursor)->next;
	}
	return (0);
}

t_command	*parse_command(t_token **tokens)
{
	t_command	*cmd;
	t_list		*args;
	t_token		*cursor;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	args = NULL;
	cursor = *tokens;
	while (cursor && cursor->type != TOKEN_PIPE && cursor->type != TOKEN_EOF)
	{
		if (process_token(&cursor, cmd, &args))
			return (NULL);
		cursor = cursor->next;
	}
	handle_pipe_recursively(&cursor, cmd);
	cmd->args = list_to_array(args);
	ft_lstclear(&args, free);
	return (cmd);
}
