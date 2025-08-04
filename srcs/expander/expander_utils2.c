/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 22:17:25 by azorlutu          #+#    #+#             */
/*   Updated: 2025/08/04 22:17:26 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_token_expansion(t_minishell *minishell,
	t_token **prev, t_token **cursor, t_env *env_list)
{
	t_token	*next;

	next = (*cursor)->next;
	if ((*cursor)->type == TOKEN_WORD && (*cursor)->quote != SINGLE_QUOTE)
	{
		if (expand_token_value(minishell, *cursor, env_list))
			return ;
	}
	if ((*cursor)->type == TOKEN_REMOVED)
	{
		if (*prev)
			(*prev)->next = next;
		else
			minishell->tokens = next;
		free((*cursor)->value);
		free(*cursor);
		*cursor = next;
	}
	else
	{
		*prev = *cursor;
		*cursor = next;
	}
}
