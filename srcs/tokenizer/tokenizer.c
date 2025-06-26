/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:44:48 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/19 21:44:49 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char *get_word_with_quotes(const char *input, int *i, t_quote_type *quote_out)
{
	char *value;
	int start;
	int len;
	char quote_char;

	quote_char = input[*i];
	if (quote_char == '\'')
		*quote_out = SINGLE_QUOTE;
	else
		*quote_out = DOUBLE_QUOTE;

	(*i)++;
	start = *i;
	len = 0;
	while (input[*i] && input[*i] != quote_char)
	{
		(*i)++;
		len++;
	}
	if (!input[*i])
	{
		ft_putstr_fd("syntax error: unexpected EOF while looking for matching quote\n", 2);
		return (NULL);
	}
	value = ft_substr(input, start, len);
	(*i)++;
	return (value);
}


static char *get_combined_token(const char *input, int *i, t_quote_type *quote_out)
{
	char *result = NULL;
	char *part = NULL;
	char *temp = NULL;
	int start;

	*quote_out = NO_QUOTE;
	while (input[*i])
	{
		if (is_whitespace(input[*i]) || is_operator(input[*i]))
			break;

		if (input[*i] == '\'' || input[*i] == '"')
		{
			part = get_word_with_quotes(input, i, quote_out);
		}
		else
		{
			start = *i;
			while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]) &&
				   input[*i] != '"' && input[*i] != '\'')
			{
				(*i)++;
			}
			part = ft_substr(input, start, *i - start);
		}
		if (!part)
		{
			free(result);
			return (NULL);
		}
		if (!result)
			result = ft_strdup(part);
		else
		{
			temp = result;
			result = ft_strjoin(result, part);
			free(temp);
		}
		free(part);
	}
	return (result);
}


t_token *tokenizer(char *input)
{
	t_token *token_list;
	t_token *new_token;
	t_quote_type quote;
	char *word_value;
	int i;
	int len;
	char *str;

	token_list = NULL;
	i = 0;
	while (input[i])
	{
		if (is_whitespace(input[i]))
			i++;
		else if (is_operator(input[i]))
		{
			len = get_token_len(get_operator_type(&input[i]));
			str = ft_substr(input, i, len);
			new_token = create_token(get_operator_type(&input[i]), str, NO_QUOTE);
			if (!new_token)
				return (NULL);
			add_token(&token_list, new_token);
			i += len;
		}
		else
		{
			word_value = get_combined_token(input, &i, &quote);
			if (!word_value)
				return (NULL);
			new_token = create_token(TOKEN_WORD, word_value, quote);
			if (!new_token)
				return (NULL);
			add_token(&token_list, new_token);
		}
	}
	return (token_list);
}