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

static t_word_info get_word_with_quotes(const char *input, int *i)
{
	t_word_info info;
	int start, len;
	char quote_char;

	quote_char = input[*i];
	info.quote = (quote_char == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;

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
		info.value = NULL;
		return info;
	}
	info.value = ft_substr(input, start, len);
	(*i)++;
	return info;
}

static t_word_info get_combined_token(const char *input, int *i)
{
	t_word_info result = {NULL, NO_QUOTE};
	t_word_info part;
	char *temp;

	while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			part = get_word_with_quotes(input, i);
		else
		{
			part.value = get_word(input, i);
			part.quote = NO_QUOTE;
		}
		if (!part.value)
		{
			free(result.value);
			result.value = NULL;
			return result;
		}
		if (!result.value)
			result.value = ft_strdup(part.value);
		else
		{
			temp = result.value;
			result.value = ft_strjoin(result.value, part.value);
			free(temp);
		}

		if (result.quote == NO_QUOTE && part.quote != NO_QUOTE)
			result.quote = part.quote;

		free(part.value);
	}
	return result;
}

t_token *tokenizer(char *input)
{
	t_token *token_list;
	t_token *new_token;
	t_word_info word_info;
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
			word_info = get_combined_token(input, &i);
			if (!word_info.value)
				return NULL;
			new_token = create_token(TOKEN_WORD, word_info.value, word_info.quote);
			if (!new_token)
				return (NULL);
			add_token(&token_list, new_token);
		}
	}
	add_token(&token_list, create_token(TOKEN_EOF, NULL, word_info.quote));
	return (token_list);
}