/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_node_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 22:09:50 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/01 22:09:51 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**list_to_array(t_list *args)
{
	int		count;
	char	**result;
	t_list	*tmp;
	int		i;

	count = ft_lstsize(args);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	tmp = args;
	i = 0;
	while (tmp)
	{
		result[i++] = ft_strdup(tmp->content);
		tmp = tmp->next;
	}
	result[i] = NULL;
	return (result);
}

int	add_arg_to_list(t_list **args, char *value)
{
	t_list	*new_node;

	if (!value)
		return (0);
	new_node = ft_lstnew(ft_strdup(value));
	if (!new_node)
	{
		perror("Error allocating memory for argument");
		return (1);
	}
	ft_lstadd_back(args, new_node);
	return (0);
}
