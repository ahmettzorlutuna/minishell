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

t_command *parse_command(t_token **tokens)
{
	t_command *cmd = ft_calloc(1, sizeof(t_command));
	t_list *args = NULL;

	while (*tokens && (*tokens)->type != TOKEN_PIPE && (*tokens)->type != TOKEN_EOF)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			ft_lstadd_back(&args, ft_lstnew(ft_strdup((*tokens)->value)));
		}
		else if ((*tokens)->type == TOKEN_REDIRECT_IN || (*tokens)->type == TOKEN_REDIRECT_OUT ||
				 (*tokens)->type == TOKEN_APPEND || (*tokens)->type == TOKEN_HEREDOC)
		{
			t_redirection *redir = ft_calloc(1, sizeof(t_redirection));
			redir->type = (*tokens)->type;
			*tokens = (*tokens)->next;
			if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
				return NULL; // syntax error
			redir->filename = ft_strdup((*tokens)->value);
			redir->next = cmd->redirects;
			cmd->redirects = redir;
		}
		*tokens = (*tokens)->next;
	}

	// pipe varsa, bir sonraki komutu parse et
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		cmd->next_pipe = parse_command(tokens);
	}

	// args linked list → char** dönüşümü
	int count = ft_lstsize(args);
	cmd->args = malloc(sizeof(char *) * (count + 1));

	t_list *tmp = args;
	for (int i = 0; i < count; i++)
	{
		cmd->args[i] = ft_strdup(tmp->content);
		tmp = tmp->next;
	}
	cmd->args[count] = NULL;

	// Bellek temizliği
	ft_lstclear(&args, free);

	return cmd;
}