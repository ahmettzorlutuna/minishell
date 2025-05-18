/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:06:24 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/18 15:06:25 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env_array(char **env_array)
{
	int	i = 0;

	if (!env_array)
		return;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

void	update_env_array(t_minishell *minishell)
{
	if (!minishell)
		return;
	free_env_array(minishell->env_array);
	minishell->env_array = env_list_to_array(minishell->env_list);
}

static  char *join_key_value(const char *key, const char *value)
{
    char *result;
    size_t key_len;
    size_t value_len;

    key_len = ft_strlen(key);
    value_len = ft_strlen(value);
    result = malloc(key_len + value_len + 2); // +2 '=' ve '\0' için
    if (!result)
        return (NULL);
    ft_strcpy(result, key);
    result[key_len] = '=';
    ft_strcpy(result + key_len + 1, value);
    return (result);
}

char	**env_list_to_array(t_env *env_list)
{
    int count;
    int i;
    t_env *tmp;
    char **env_array;

    count = 0;
    i = 0;
    tmp = env_list;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    env_array = malloc(sizeof(char *) * (count + 1));
    if(!env_array)
        return (NULL);
    tmp = env_list;
    while(tmp)
    {
        env_array[i] = join_key_value(tmp->key, tmp->value);
        if(!env_array[i])
        {
            while (i > 0)
            {
                free(env_array[i - 1]);
                i--;
            }
            free(env_array);
            return (NULL);
        }
        i++;
        tmp = tmp->next;
    }
    env_array[i] = NULL;
    return (env_array);
}