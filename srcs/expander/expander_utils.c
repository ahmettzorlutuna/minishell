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
	int		start;
	int		len;
	char	*variable_name;
	char	*variable_value;

	start = *(ctx->i);
	while (ctx->token_value[*(ctx->i)] &&
		(ft_isalnum(ctx->token_value[*(ctx->i)]) || ctx->token_value[*(ctx->i)] == '_'))
		(*(ctx->i))++;
	len = *(ctx->i) - start;
	if (len == 0)
		return (NULL);
	variable_name = ft_substr(ctx->token_value, start, len);
	variable_value = get_env_value(env_list, variable_name);
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

int	expand_token_value(t_minishell *minishell, t_token *cursor, t_env *env_list)
{
	t_token_part	*part;
	char			*expanded;
	char			*joined;

	joined = ft_strdup("");
	if (!joined)
		return (1);
	part = cursor->parts;
	while (part)
	{
		if (part->quote == SINGLE_QUOTE)
			expanded = ft_strdup(part->str);
		else
			expanded = expand_word(minishell, part->quote, part->str, env_list);
		if (!expanded)
		{
			free(joined);
			return (1);
		}
		joined = ft_strjoin_free(joined, expanded);
		free(expanded);
		part = part->next;
	}
	free(cursor->value);
	cursor->value = joined;
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
