/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 17:42:29 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/17 17:42:30 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	unset_env_value(t_env **env_list, const char *key)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env_list;
	prev = NULL;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env_list = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	set_env_value(t_env **env_list, const char *key, const char *value)
{
	t_env	*tmp;
	t_env	*new_node;

	// Eğer varsa güncelle
	tmp = *env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}

	// Yoksa sona ekle
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;

	if (!*env_list)
	{
		*env_list = new_node;
		return ;
	}

	tmp = *env_list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

char	*get_env_value(t_env *env_list, const char *key)
{
	while (env_list)
	{
		if (ft_strcmp(env_list->key, key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

static	t_env	*create_env_node(char *env_str)
{
	t_env	*node;
	char	*equal_sign;

	equal_sign = ft_strchr(env_str, '=');
	if (!equal_sign)
		return (NULL);
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_substr(env_str, 0, equal_sign - env_str);
	node->value = ft_strdup(equal_sign + 1);
	node->next = NULL;
	return (node);
}

t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*new_node;

	head = NULL;
	tail = NULL;
	while (*envp)
	{
		new_node = create_env_node(*envp);
		if (!new_node)
			continue;
		if (!head)
			head = new_node;
		else
			tail->next = new_node;
		tail = new_node;
		envp++;
	}
	return (head);
}
