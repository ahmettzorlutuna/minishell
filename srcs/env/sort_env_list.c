/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 22:12:16 by azorlutu          #+#    #+#             */
/*   Updated: 2025/06/03 22:12:17 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env	*copy_env_node(t_env *src)
{
	t_env	*copy;

	copy = (t_env *)malloc(sizeof(t_env));
	if (!copy)
		return (NULL);
	copy->key = ft_strdup(src->key);
	copy->value = src->value ? ft_strdup(src->value) : NULL;
	copy->next = NULL;
	return (copy);
}

static t_env	*copy_env_list(t_env *src)
{
	t_env	*new_head;
	t_env	*new_tail;
	t_env	*new_node;

    new_tail = NULL;
    new_head = NULL;
	while (src)
	{
		new_node = copy_env_node(src);
		if (!new_node)
			return (free_env_list(new_head), NULL);
		if (!new_head)
			new_head = new_node;
		else
			new_tail->next = new_node;
		new_tail = new_node;
		src = src->next;
	}
	return (new_head);
}

static void	swap_nodes(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_value;

    tmp_key = a->key;
    tmp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = tmp_key;
	b->value = tmp_value;
}

t_env	*sort_env_list(t_env *env)
{
	t_env	*sorted;
	t_env	*curr;
    t_env   *next;

	sorted = copy_env_list(env);
	if (!sorted)
		return (NULL);
	curr = sorted;
	while (curr)
	{
		next = curr->next;
		while (next)
		{
			if (ft_strcmp(curr->key, next->key) > 0)
				swap_nodes(curr, next);
			next = next->next;
		}
		curr = curr->next;
	}
	return (sorted);
}