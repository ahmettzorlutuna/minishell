/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 19:49:00 by azorlutu          #+#    #+#             */
/*   Updated: 2024/10/08 19:49:01 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	unsigned char		*dest;
	const unsigned char	*source;

	source = (unsigned char *)src;
	dest = (unsigned char *)dst;
	if (dest == NULL && source == NULL)
	{
		return (0);
	}
	if (source < dest)
	{
		while (n > 0)
		{
			n--;
			dest[n] = source[n];
		}
	}
	else
	{
		ft_memcpy(dest, src, n);
	}
	return (dst);
}
