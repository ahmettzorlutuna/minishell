/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 20:30:44 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/01 20:30:45 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	determine_quote_type(char c, t_quote_type *quote_out)
{
	if (c == '\'')
		*quote_out = SINGLE_QUOTE;
	else
		*quote_out = DOUBLE_QUOTE;
}

int	measure_quoted_length(const char *input, int *i, char quote_char)
{
	int	len;

	len = 0;
	// printf("→ measure_quoted_length starting at: %c\n", input[*i]);
	while (input[*i] && input[*i] != quote_char)
	{
		(*i)++;
		len++;
	}
	return (len);
}

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] && is_whitespace(input[*i]))
		(*i)++;
}
