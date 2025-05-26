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

static char **list_to_array(t_list *args)
{
	int count;
	char **result;
	t_list *tmp;
	int i;

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

static int parse_redirection(t_token **tokens, t_command *cmd)
{
	t_redirection *redir;
	t_redirection **redir_ptr;

	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
		return (1); // Syntax error.
	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return (1);
	redir->type = (*tokens)->type;
	redir->filename = ft_strdup((*tokens)->next->value);
	redir_ptr = &cmd->redirects;
	while (*redir_ptr)
		redir_ptr = &(*redir_ptr)->next;
	*redir_ptr = redir;
	(*tokens) = (*tokens)->next;
	return (0);
}

static int add_arg_to_list(t_list **args, char *value)
{
	t_list *new_node;

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

t_command *parse_command(t_token **tokens)
{
	t_command *cmd;
	t_list *args;

	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
	{
		perror("Error allocating memory for command");
		return NULL;
	}
	args = NULL;
	while (*tokens && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
	{
		if ((*tokens) && (*tokens)->type == TOKEN_WORD)
			add_arg_to_list(&args, (*tokens)->value);
		else if ((*tokens) && ((*tokens)->type == TOKEN_REDIRECT_IN ||
							   (*tokens)->type == TOKEN_REDIRECT_OUT ||
							   (*tokens)->type == TOKEN_HEREDOC ||
							   (*tokens)->type == TOKEN_APPEND))
		{
			if (parse_redirection(tokens, cmd))
				return (NULL);
		}
		(*tokens) = (*tokens)->next;
	}
	if ((*tokens) && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		cmd->next_pipe = parse_command(tokens);
	}
	cmd->args = list_to_array(args);
	ft_lstclear(&args, free);
	return (cmd);
}