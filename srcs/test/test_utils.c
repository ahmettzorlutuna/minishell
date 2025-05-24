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