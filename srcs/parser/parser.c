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

t_quote_type	get_heredoc_quote_type(t_token *token)
{
	t_token_part	*part;

	if (!token || !token->parts)
		return (NO_QUOTE);

	part = token->parts;

	// Eğer sadece bir part varsa onun quote'u belirleyicidir
	if (part->next == NULL)
		return (part->quote);

	// Birden fazla part varsa heredoc expand edilir
	return (NO_QUOTE);
}

static int	init_heredoc_redir(t_minishell *minishell,
				t_redirection *redir, t_token *token)
{
	redir->delimiter_raw = ft_strdup(token->value);
	if (!redir->delimiter_raw)
		return (1);

	redir->redir_quote = get_heredoc_quote_type(token);

	if (redir->redir_quote == NO_QUOTE)
	{
		redir->delimiter_expanded = expand_word(minishell,
				NO_QUOTE, redir->delimiter_raw, minishell->env_list);
	}
	else
	{
		redir->delimiter_expanded = ft_strdup(redir->delimiter_raw);
	}

	if (!redir->delimiter_expanded)
	{
		free(redir->delimiter_raw);
		return (1);
	}
	return (0);
}

static int	init_filename_redir(t_redirection *redir, t_token *token)
{
	redir->filename = ft_strdup(token->value);
	if (!redir->filename)
		return (1);
	return (0);
}

static int	parse_redirection(t_minishell *minishell,
				t_token *cursor, t_command *cmd)
{
	t_redirection	*redir;
	t_redirection	**redir_ptr;

	if (!cursor->next || cursor->next->type != TOKEN_WORD)
	{
		free_command_list(cmd);
		return (1);
	}
	redir = ft_calloc(1, sizeof(t_redirection));
	if (!redir)
		return (1);
	redir->fd = -1;
	redir->type = cursor->type;
	if (cursor->type == TOKEN_HEREDOC)
	{
		if (init_heredoc_redir(minishell, redir, cursor->next))
			return (free_redirections(redir), 1);
	}
	else if (init_filename_redir(redir, cursor->next))
		return (free_redirections(redir), 1);
	redir_ptr = &cmd->redirects;
	while (*redir_ptr)
		redir_ptr = &(*redir_ptr)->next;
	*redir_ptr = redir;
	return (0);
}

char	*join_token_parts(t_minishell *minishell, t_token_part *parts)
{
	t_token_part	*current = parts;
	char			*result;
	char			*expanded;
	char			*tmp;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (current)
	{
		if (current->quote == SINGLE_QUOTE)
			expanded = ft_strdup(current->str);
		else
			expanded = expand_word(minishell, current->quote,
					current->str, minishell->env_list);
		if (!expanded)
		{
			free(result);
			return (NULL);
		}
		tmp = ft_strjoin(result, expanded);
		free(result);
		free(expanded);
		result = tmp;
		current = current->next;
	}
	return (result);
}

static int	process_token(t_minishell *minishell,
				t_token **cursor, t_command *cmd, t_list **args)
{
	char	*value;

	if ((*cursor)->type == TOKEN_WORD)
	{
		value = join_token_parts(minishell, (*cursor)->parts);
		if (!value)
			return (1);
		add_arg_to_list(args, value);
		free(value);
	}
	else if ((*cursor)->type == TOKEN_REDIRECT_IN
		|| (*cursor)->type == TOKEN_REDIRECT_OUT
		|| (*cursor)->type == TOKEN_HEREDOC
		|| (*cursor)->type == TOKEN_APPEND)
	{
		if (parse_redirection(minishell, *cursor, cmd))
			return (1);
		*cursor = (*cursor)->next;
	}
	return (0);
}

t_command	*parse_command(t_minishell *minishell, t_token **tokens)
{
	t_command	*cmd;
	t_list		*args;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	args = NULL;

	while (*tokens && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
	{
		if (process_token(minishell, tokens, cmd, &args))
		{
			free_arg_list(args);
			return (NULL);
		}
		*tokens = (*tokens)->next;
	}
	handle_pipe_recursively(minishell, tokens, cmd);
	cmd->args = list_to_array(args);
	free_arg_list(args);
	return (cmd);
}
