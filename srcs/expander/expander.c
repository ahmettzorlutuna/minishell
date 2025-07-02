/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 18:31:21 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/23 18:31:23 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	void	expand_variable(
	t_expand_ctx *ctx,
	t_env *env_list,
	char **result)
{
	char	*variable_value;
	char	*temp;

	variable_value = get_variable_value(ctx, env_list);
	if (!variable_value)
	{
		temp = ft_strdup("");
		*result = ft_strjoin_free(*result, temp);
		free(temp);
	}
	else
		*result = ft_strjoin_free(*result, variable_value);
}

static	int	append_literal_char(char *token_value, int *i, char **result)
{
	char	*new_result;

	new_result = ft_substr(token_value, *i, 1);
	if (!new_result)
		return (1);
	*result = ft_strjoin_free(*result, new_result);
	free(new_result);
	(*i)++;
	return (0);
}

static	void	expand_dollar(t_minishell *minishell,
						t_expand_ctx *ctx, t_env *env_list, char **result)
{
	if (ctx->token_value[*(ctx->i)] == '?')
	{
		expand_exit_status(minishell, result, ctx->i);
	}
	else if (ctx->token_value[*(ctx->i)] == '\0'
		|| (!ft_isalnum(ctx->token_value[*(ctx->i)])
			&& ctx->token_value[*(ctx->i)] != '_'))
	{
		*result = ft_strjoin_free(*result, "$");
	}
	else
	{
		expand_variable(ctx, env_list, result);
	}
}

char	*expand_word(t_minishell *minishell,
						t_quote_type quote_type,
						char *token_value, t_env *env_list)
{
	char			*result;
	int				i;
	t_expand_ctx	ctx;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	ctx.token_value = token_value;
	ctx.i = &i;
	ctx.quote_type = quote_type;
	while (token_value[i])
	{
		if (token_value[i] == '$')
		{
			i++;
			expand_dollar(minishell, &ctx, env_list, &result);
		}
		else if (append_literal_char(token_value, &i, &result))
			return (free(result), NULL);
	}
	return (result);
}

void	expand_tokens(t_minishell *minishell,
			t_token *token_list, t_env *env_list)
{
	t_token	*prev;
	t_token	*cursor;

	prev = NULL;
	cursor = token_list;
	while (cursor)
	{
		if (is_heredoc_delimiter(prev))
		{
			prev = cursor;
			cursor = cursor->next;
			continue ;
		}
		if (cursor->type == TOKEN_WORD && cursor->quote != SINGLE_QUOTE)
		{
			if (expand_token_value(minishell, cursor, env_list))
				return ;
		}
		prev = cursor;
		cursor = cursor->next;
	}
}
