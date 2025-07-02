/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:01:35 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/23 19:01:38 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_variable_value(t_expand_ctx *ctx, t_env *env_list)
{
	char	*variable_name;
	char	*variable_value;
	int		start;

	start = *(ctx->i);
	while ((ctx->token_value[*(ctx->i)]
			&& ft_isalnum(ctx->token_value[*(ctx->i)]))
		|| ctx->token_value[*(ctx->i)] == '_')
		(*(ctx->i))++;
	variable_name = ft_substr(ctx->token_value, start, *(ctx->i) - start);
	variable_value = get_env_value(env_list, variable_name);
	while (!variable_value && *(ctx->i) > start && ctx->quote_type != NO_QUOTE)
	{
		(*(ctx->i))--;
		free(variable_name);
		variable_name = ft_substr(ctx->token_value, start, *(ctx->i) - start);
		variable_value = get_env_value(env_list, variable_name);
	}
	free(variable_name);
	return (variable_value);
}

void	expand_exit_status(t_minishell *minishell,
					char **result, int *i)
{
	char	*exit_status;

	exit_status = ft_itoa(minishell->last_exit_code);
	*result = ft_strjoin_free(*result, exit_status);
	free(exit_status);
	(*i)++;
}

int	expand_token_value(
	t_minishell *minishell,
	t_token *cursor,
	t_env *env_list)
{
	char	*expanded_word;

	expanded_word = expand_word(minishell,
			cursor->quote, cursor->value, env_list);
	if (!expanded_word)
	{
		free(cursor->value);
		cursor->value = NULL;
		return (1);
	}
	free(cursor->value);
	cursor->value = expanded_word;
	return (0);
}

int	is_heredoc_delimiter(t_token *prev)
{
	if (prev && prev->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*new;

	new = ft_strjoin(s1, s2);
	free(s1);
	return (new);
}
