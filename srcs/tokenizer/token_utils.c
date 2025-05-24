/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 01:01:55 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/23 01:01:56 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

int is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char *get_word(const char *input, int *i)
{
	int start;
	int len;

	start = *i;
	len = 0;
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
	{
		(*i)++;
		len++;
	}
	return (ft_substr(input, start, len));
}

t_token_type get_operator_type(const char *input)
{
	if (input[0] == '<' && input[1] == '<')
		return (TOKEN_HEREDOC);
	if (input[0] == '>' && input[1] == '>')
		return (TOKEN_APPEND);
	if (input[0] == '<')
		return (TOKEN_REDIRECT_IN);
	if (input[0] == '>')
		return (TOKEN_REDIRECT_OUT);
	if (input[0] == '|')
		return (TOKEN_PIPE);
	return (TOKEN_EOF);
}

int get_token_len(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT || type == TOKEN_PIPE)
		return (1);
	if (type == TOKEN_HEREDOC || type == TOKEN_APPEND)
		return (2);
	return (0);
}