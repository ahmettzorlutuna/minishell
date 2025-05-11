/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 16:43:09 by azorlutu          #+#    #+#             */
/*   Updated: 2024/10/12 16:43:10 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_all(char **result)
{
	int	i;

	i = 0;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (NULL);
}

static	int	count_words(const char *s, int c)
{
	size_t	count;
	size_t	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	int		k;
	int		j;
	char	**res;

	res = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!res || !s)
		return (NULL);
	k = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			j = 0;
			while (s[j] && s[j] != c)
				j++;
			res[k] = ft_substr(s, 0, j);
			if (res[k++] == NULL)
				return (free_all(res));
			s += j;
		}
	}
	res[k] = NULL;
	return (res);
}
