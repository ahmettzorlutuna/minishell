/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 11:27:12 by azorlutu          #+#    #+#             */
/*   Updated: 2024/10/12 11:27:13 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	int		s1_len;

	s1_len = ft_strlen(s1);
	ptr = malloc(s1_len + 1);
	if (ptr)
		ft_memcpy(ptr, s1, s1_len + 1);
	else
		return (NULL);
	return (ptr);
}
