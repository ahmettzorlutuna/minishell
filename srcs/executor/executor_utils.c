/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 00:10:57 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/28 00:10:57 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_split(char **split)
{
    int i;

    if(!split)
        return ;
    i = 0;
    while (split[i])
    {
        free(split[i]);
        i++;
    }
    free(split);
}

char *ft_strjoin_three(const char *s1, const char *s2, const char *s3)
{
    char *first;
    char *full;

    if(!*s1 || !*s2 || !*s3)
        return (NULL);

    first = ft_strjoin(s1, s2);
    if(!first)
        return (NULL);
    full = ft_strjoin(first, s3);
    free(first);
    return (full);
}

