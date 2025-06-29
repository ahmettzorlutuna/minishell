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

static	char	**list_to_array(t_list *args)
{
	int		count;
	char	**result;
	t_list	*tmp;
	int		i;

	count = ft_lstsize(args);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	tmp = args;
	i = 0;
	while (tmp)
	{
		result[i++] = ft_strdup(tmp->content);
		tmp = tmp->next;
	}
	result[i] = NULL;
	return (result);
}

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

static	int	add_arg_to_list(t_list **args, char *value)
{
	t_list	*new_node;

	if (!value || !*value)
		return (0);
	new_node = ft_lstnew(ft_strdup(value));
	if (!new_node)
	{
		perror("Error allocating memory for argument");
		return (1);
	}
	ft_lstadd_back(args, new_node);
	return (0);
}

t_command	*parse_command(t_token **tokens)
{
	t_command	*cmd;
	t_list		*args;
	t_token		*cursor;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
	{
		perror("Error allocating memory for command");
		return (NULL);
	}
	args = NULL;
	cursor = *tokens;
	while (cursor && cursor->type != TOKEN_PIPE && cursor->type != TOKEN_EOF)
	{
		if (cursor->type == TOKEN_WORD)
			add_arg_to_list(&args, cursor->value);
		else if (cursor && (cursor->type == TOKEN_REDIRECT_IN
				|| cursor->type == TOKEN_REDIRECT_OUT
				|| cursor->type == TOKEN_HEREDOC
				|| cursor->type == TOKEN_APPEND))
		{
			if (parse_redirection(cursor, cmd))
				return (NULL);
			cursor = cursor->next;
		}
		cursor = cursor->next;
	}
	if (cursor && cursor->type == TOKEN_PIPE)
	{
		cursor = cursor->next;
		cmd->next_pipe = parse_command(&cursor);
	}
	cmd->args = list_to_array(args);
	ft_lstclear(&args, free);
	return (cmd);
}
