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

static char *get_word_with_quotes(const char *input, int *i)
{
	int start, len;
	char quote;
	char *quoted_part;
	char *rest_part;
	char *final_word;

	len = 0;
	quote = input[*i];

	(*i)++; // açılış tırnağını geç
	start = *i;
	while (input[*i] && input[*i] != quote)
	{
		(*i)++;
		len++;
	}
	if (!input[*i])
	{
		ft_putstr_fd("syntax error: unexpected EOF while looking for matching quote\n", 2);
		return (NULL);
	}
	quoted_part = ft_substr(input, start, len); // örn: ahmet
	(*i)++;										// kapanış tırnağını geç
	start = *i;
	len = 0;
	while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\'' && input[*i] != '"')
	{
		(*i)++;
		len++;
	}
	rest_part = ft_substr(input, start, len); // örn: 123
	final_word = ft_strjoin(quoted_part, rest_part);
	free(quoted_part);
	free(rest_part);
	return (final_word);
}

static char *get_combined_token(const char *input, int *i)
{
	char *part;
	char *combined = NULL;
	char *temp;

	while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			part = get_word_with_quotes(input, i);
		else
			part = get_word(input, i);
		if (!part)
		{
			free(combined);
			return (NULL);
		}
		temp = combined;
		if (!combined)
			combined = ft_strdup(part);
		else
		{
			combined = ft_strjoin(combined, part); // birleştir
			free(temp);
			free(part);
		}
	}
	return (combined);
}

t_token *tokenizer(char *input)
{
	t_token *token_list;
	t_token *new_token;
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
			new_token = create_token(get_operator_type(&input[i]), str);
			if (!new_token)
				return (NULL);
			add_token(&token_list, new_token);
			i += len;
		}
		else
		{
			str = get_combined_token(input, &i);
			if (!str)
				return (NULL);
			new_token = create_token(TOKEN_WORD, str);
			if (!new_token)
				return (NULL);
			add_token(&token_list, new_token);
		}
	}
	add_token(&token_list, create_token(TOKEN_EOF, NULL));
	return (token_list);
}