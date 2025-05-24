/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 12:15:06 by azorlutu          #+#    #+#             */
/*   Updated: 2024/10/12 12:15:07 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_check_s_len(size_t s_len, unsigned int start, size_t len)
{
	if (s_len - start < len)
		len = s_len - start;
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;
	size_t	j;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	len = ft_check_s_len(s_len, start, len);
	ptr = malloc((len + 1) * sizeof(*s));
	if (!ptr)
		return (NULL);
	i = 0;
	j = 0;
	while (i < start && s[i] != '\0')
		i++;
	while (j < len)
	{
		ptr[j] = s[i + j];
		j++;
	}
	ptr[j] = '\0';
	return (ptr);
}
