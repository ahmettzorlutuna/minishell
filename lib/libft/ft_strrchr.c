/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:10:28 by azorlutu          #+#    #+#             */
/*   Updated: 2024/10/10 14:10:29 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	s_len;

	s_len = ft_strlen(s);
	if ((char)c == 0)
		return ((char *)s + s_len);
	while (s_len >= 0)
	{
		if (s[s_len] == (char)c)
			return ((char *) s + s_len);
		s_len--;
	}
	return (0);
}
