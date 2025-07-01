/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_combined_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:41:19 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/01 20:41:20 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_word_with_quotes(const char *input,
					int *i, t_quote_type *quote_out)
{
	char	*value;
	int		start;
	int		len;
	char	quote_char;

	quote_char = input[*i];
	determine_quote_type(quote_char, quote_out);
	(*i)++;
	start = *i;
	len = measure_quoted_length(input, i, quote_char);
	if (!input[*i])
	{
		ft_putstr_fd("syntax error: unexpected EOF while" \
				"looking for matching quote\n", 2);
		return (NULL);
	}
	value = ft_substr(input, start, len);
	(*i)++;
	return (value);
}

static char	*extract_part(const char *input, int *i, t_quote_type *quote_out)
{
	char	*part;
	int		start;

	part = NULL;
	if (input[*i] == '\'' || input[*i] == '"')
		part = get_word_with_quotes(input, i, quote_out);
	else
	{
		start = *i;
		while (input[*i] && !is_whitespace(input[*i])
			&& !is_operator(input[*i])
			&& input[*i] != '"' && input[*i] != '\'')
			(*i)++;
		part = ft_substr(input, start, *i - start);
		*quote_out = NO_QUOTE;
	}
	return (part);
}

static int	append_to_result(char **result, char *part)
{
	char	*temp;

	if (!*result)
	{
		*result = ft_strdup(part);
		if (!*result)
			return (1);
	}
	else
	{
		temp = *result;
		*result = ft_strjoin(*result, part);
		free(temp);
		if (!*result)
			return (1);
	}
	return (0);
}

char	*get_combined_token(const char *input,
					int *i, t_quote_type *quote_out)
{
	char	*result;
	char	*part;

	result = NULL;
	*quote_out = NO_QUOTE;
	while (input[*i])
	{
		if (is_whitespace(input[*i]) || is_operator(input[*i]))
			break ;
		part = extract_part(input, i, quote_out);
		if (!part)
		{
			free(result);
			return (NULL);
		}
		if (append_to_result(&result, part))
		{
			free(part);
			return (NULL);
		}
		free(part);
	}
	return (result);
}
