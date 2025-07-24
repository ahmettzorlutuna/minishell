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

static int	process_operator_token(char *input, int *i, t_token **token_list)
{
	int			len;
	char		*str;
	t_token		*new_token;

	t_token_type type = get_operator_type(&input[*i]);
	len = get_token_len(type);
	str = ft_substr(input, *i, len);
	if (!str)
		return (1);

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (free(str), 1);

	new_token->type = type;
	new_token->value = str;
	new_token->parts = NULL;
	new_token->next = NULL;

	add_token(token_list, new_token);
	*i += len;
	return (0);
}

// static int	process_word_token(char *input, int *i, t_token **token_list)
// {
// 	char			*word_value;
// 	t_token			*new_token;
// 	t_quote_type	quote;

// 	word_value = get_combined_token(input, i, &quote);
// 	if (!word_value)
// 		return (1);
// 	new_token = create_token(TOKEN_WORD, word_value, quote);
// 	if (!new_token)
// 	{
// 		free(word_value);
// 		return (1);
// 	}
// 	add_token(token_list, new_token);
// 	return (0);
// }

static int	process_word_token(char *input, int *i, t_token **token_list)
{
	t_token_part	*parts;
	t_token			*new_token;

	parts = extract_one_token_parts(input, i);
	if (!parts)
		return (1);

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free_token_parts(parts);
		return (1);
	}
	new_token->type = TOKEN_WORD;
	new_token->parts = parts;
	new_token->value = NULL;
	new_token->next = NULL;

	add_token(token_list, new_token);
	return (0);
}

t_token	*tokenizer(char *input, t_minishell *minishell)
{
	t_token	*token_list;
	int		i;

	(void)minishell;
	token_list = NULL;
	i = 0;
	while (input[i])
	{
		skip_whitespace(input, &i);
		if (input[i] && is_operator(input[i]))
		{
			if (process_operator_token(input, &i, &token_list))
				return (free_token_list(token_list), NULL);
		}
		else if (input[i])
		{
			if (process_word_token(input, &i, &token_list))
				return (free_token_list(token_list), NULL);
		}	
	}
	return (token_list);
}
