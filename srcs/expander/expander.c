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

static char *expand_word(t_minishell *minishell, t_quote_type quote_type, char *token_value, t_env *env_list)
{
	char *result;
	char *new_result;
	char *variable_name;
	char *variable_value;
	char *exit_status;
	char *temp;
	int start;
	int i;

	result = ft_strdup("");
	i = 0;
	while (token_value[i])
	{
		if (token_value[i] == '$')
		{
			i++;
			if (token_value[i] == '?')
			{
				exit_status = ft_itoa(minishell->last_exit_code);
				free(result);
				result = ft_strjoin(result, exit_status);
				free(exit_status);
				i++;
			}
			else if (token_value[i] == '\0' || (!ft_isalnum(token_value[i]) && token_value[i] != '_'))
			{
				temp = ft_strdup("$");
				result = ft_strjoin_free(result, temp);
				free(temp);
			}
			else
			{
				start = i;
				while ((token_value[i] && ft_isalnum(token_value[i])) || token_value[i] == '_')
					i++;
				variable_name = ft_substr(token_value, start, i - start);
				variable_value = get_env_value(env_list, variable_name);
				while (!variable_value && i > start && quote_type != NO_QUOTE)
				{
					i--;
					free(variable_name);
					variable_name = ft_substr(token_value, start, i - start);
					variable_value = get_env_value(env_list, variable_name);
				}
				if (!variable_value)
				{
					temp = ft_strdup("");
					result = ft_strjoin_free(result, temp);
					free(variable_value);
					free(temp);
				}
				else
					result = ft_strjoin_free(result, variable_value);
				free(variable_name);
			}
		}
		else
		{
			new_result = ft_substr(token_value, i, 1);
			if(!new_result)
			{
				free(result);
				return (NULL);
			}
			result = ft_strjoin_free(result, new_result);
			free(new_result);
			i++;
		}
	}
	if(!result)
		return (NULL);
	return (result);
}

void expand_tokens(t_minishell *minishell, t_token *token_list, t_env *env_list)
{
	char *expanded_word;

	while (token_list)
	{
		if (token_list->type == TOKEN_WORD && token_list->quote != SINGLE_QUOTE)
		{
			expanded_word = expand_word(minishell, token_list->quote, token_list->value, env_list);
			if (!expanded_word)
			{
				free(token_list->value);
				token_list->value = NULL;
				return;
			}
			free(token_list->value);
			token_list->value = expanded_word;
		}
		token_list = token_list->next;
	}
}