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

	// printf("**************************************");
	// printf("\n");
	// printf("→ get_word_with_quotes: input = %s\n", &input[*i]);
	quote_char = input[*i];
	determine_quote_type(quote_char, quote_out);
	(*i)++;
	start = *i;
	len = measure_quoted_length(input, i, quote_char);
	// printf("→ measured len = %d\n", len);
	if (!input[*i])
		return (NULL);
	value = ft_substr(input, start, len);
	(*i)++;
	return (value);
}

static	char	*get_word_until_quote_or_space(const char *input, int *i, t_quote_type *quote_out)
{
	int	start;

	start = *i;
	while (input[*i] && !is_whitespace(input[*i])
		&& !is_operator(input[*i])
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	*quote_out = NO_QUOTE;
	return (ft_substr(input, start, *i - start));
}

t_token_part	*extract_one_token_parts(const char *input, int *i)
{
	t_token_part	*head = NULL;
	t_token_part	*last = NULL;
	t_token_part	*new_part;
	t_quote_type	quote;
	char			*part;

	while (input[*i]
		&& input[*i] != ' ' && input[*i] != '\t'
		&& !is_operator(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			part = get_word_with_quotes(input, i, &quote);
		else
			part = get_word_until_quote_or_space(input, i, &quote);
		if (!part)
			break;
		new_part = malloc(sizeof(t_token_part));
		if (!new_part)
			return (free_token_parts(head), NULL);
		new_part->str = part;
		new_part->quote = quote;
		new_part->next = NULL;
		if (!head)
			head = new_part;
		else
			last->next = new_part;
		last = new_part;

		// ❗️ Burada dikkat: boşluk veya operator geldiyse döngüyü kır
		if (is_whitespace(input[*i]) || is_operator(input[*i]))
			break;
	}
	return (head);
}


// static int	append_to_result(char **result, char *part)
// {
// 	char	*temp;

// 	if (!*result)
// 	{
// 		*result = ft_strdup(part);
// 		if (!*result)
// 			return (1);
// 	}
// 	else
// 	{
// 		temp = *result;
// 		*result = ft_strjoin(*result, part);
// 		free(temp);
// 		if (!*result)
// 			return (1);
// 	}
// 	return (0);
// }

// char	*get_combined_token(const char *input,
// 					int *i, t_quote_type *quote_out)
// {
// 	char	*result;
// 	char	*part;

// 	result = NULL;
// 	*quote_out = NO_QUOTE;
// 	while (input[*i])
// 	{
// 		if (is_whitespace(input[*i]) || is_operator(input[*i]))
// 			break ;
// 		part = extract_token_parts(input, i);
// 		if (!part)
// 		{
// 			free(result);
// 			return (NULL);
// 		}
// 		if (append_to_result(&result, part))
// 		{
// 			free(part);
// 			return (NULL);
// 		}
// 		free(part);
// 	}
// 	return (result);
// }
