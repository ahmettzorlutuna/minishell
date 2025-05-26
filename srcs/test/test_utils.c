/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:49:30 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/19 23:49:31 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static const char *token_type_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return "WORD";
	else if (type == TOKEN_REDIRECT_IN)
		return "REDIRECT_IN (<)";
	else if (type == TOKEN_REDIRECT_OUT)
		return "REDIRECT_OUT (>)";
	else if (type == TOKEN_HEREDOC)
		return "HEREDOC (<<)";
	else if (type == TOKEN_APPEND)
		return "APPEND (>>)";
	else if (type == TOKEN_PIPE)
		return "PIPE (|)";
	else if (type == TOKEN_EOF)
		return "EOF";
	return "UNKNOWN";
}

static const char *quote_type_str(t_quote_type quote)
{
	if (quote == NO_QUOTE)
		return "NO_QUOTE";
	else if (quote == SINGLE_QUOTE)
		return "SINGLE_QUOTE";
	else if (quote == DOUBLE_QUOTE)
		return "DOUBLE_QUOTE";
	return "UNKNOWN";
}

void print_tokens(t_token *tokens)
{
	while (tokens)
	{
		printf("Token Type: %-16s | Value: %-12s | Quote: %s\n",
			token_type_str(tokens->type),
			tokens->value ? tokens->value : "NULL",
			quote_type_str(tokens->quote));
		tokens = tokens->next;
	}
}

void	print_parsed_command(t_command *cmd)
{
	int		i;
	t_redirection *redir;

	while (cmd)
	{
		printf("🟢 Komut:\n");

		// Argümanlar
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				printf("  🔹 Arg[%d]: %s\n", i, cmd->args[i]);
				i++;
			}
		}
		else
			printf("  ⚠️  Argüman yok.\n");

		// Redirection'lar
		redir = cmd->redirects;
		while (redir)
		{
			const char *redir_type = (
				redir->type == TOKEN_REDIRECT_IN ? "<" :
				redir->type == TOKEN_REDIRECT_OUT ? ">" :
				redir->type == TOKEN_APPEND ? ">>" :
				redir->type == TOKEN_HEREDOC ? "<<" : "?"
			);
			printf("  🔁 Redirection: %s %s\n", redir_type, redir->filename);
			redir = redir->next;
		}

		// Sonraki pipe varsa
		cmd = cmd->next_pipe;
		if (cmd)
			printf("🔗 PIPE →\n");
	}
}

static const char *redir_type_to_str(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN)
		return "REDIRECT_IN (<)";
	if (type == TOKEN_REDIRECT_OUT)
		return "REDIRECT_OUT (>)";
	if (type == TOKEN_APPEND)
		return "APPEND (>>)";
	if (type == TOKEN_HEREDOC)
		return "HEREDOC (<<)";
	return "UNKNOWN";
}

void	print_command(t_command *cmd)
{
	int				i;
	t_redirection	*redir;

	while (cmd)
	{
		printf("🟢 Komut:\n");

		// Argümanlar
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				printf("  🔹 Arg[%d]: \"%s\"\n", i, cmd->args[i]);
				i++;
			}
		}
		else
			printf("  ⚠️  Argüman yok.\n");

		// Redirection'lar
		redir = cmd->redirects;
		while (redir)
		{
			printf("  🔁 Redirection:\n");
			printf("     - Type    : %s\n", redir_type_to_str(redir->type));
			printf("     - Filename: \"%s\"\n", redir->filename);
			redir = redir->next;
		}

		cmd = cmd->next_pipe;
		if (cmd)
			printf("🔗 PIPE →\n");
	}
}