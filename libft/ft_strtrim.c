/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 14:59:39 by azorlutu          #+#    #+#             */
/*   Updated: 2024/10/12 14:59:39 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start_index;
	int		end_index;
	int		s1_length;
	char	*str;

	start_index = 0;
	while (s1[start_index] && ft_strchr(set, s1[start_index]))
		start_index++;
	end_index = ft_strlen(s1);
	while (end_index > start_index && ft_strchr(set, s1[end_index - 1]))
		end_index--;
	s1_length = end_index - start_index;
	str = malloc(sizeof(char) * (s1_length + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, &s1[start_index], s1_length + 1);
	return (str);
}
