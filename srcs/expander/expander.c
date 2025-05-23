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

static char *expand_word(char *token_value, t_env *env_list)
{
	char *result;
	char *new_result;
	char *variable_name;
	char *variable_value;
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
				char *exit_status = ft_itoa(g_signal_flag);
				result = ft_strjoin_free(result, exit_status);
				i++;
			}
			else
			{
				start = i;
				while ((token_value[i] && ft_isalnum(token_value[i])) || token_value[i] == '_')
					i++;
				variable_name = ft_substr(token_value, start, i - start);
				variable_value = get_env_value(env_list, variable_name);
				if(!variable_value)
					variable_value = "";
				result = ft_strjoin_free(result, variable_value);
				free(variable_name);
			}
		}
		else
		{
			new_result = ft_substr(token_value, i, 1);
			result = ft_strjoin_free(result, new_result);
			i++;
		}
	}
	return (result);
}

void expand_tokens(t_token *token_list, t_env *env_list)
{
	char *expanded_word;

	while (token_list)
	{
		if (token_list->type == TOKEN_WORD)
		{
			expanded_word = expand_word(token_list->value, env_list);
			if (!expanded_word)
				return;
			free(token_list->value);
			token_list->value = expanded_word;
		}
		token_list = token_list->next;
	}
}