/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 15:22:09 by azorlutu          #+#    #+#             */
/*   Updated: 2024/10/10 15:22:10 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*uns_s1;
	unsigned char	*uns_s2;
	size_t			i;

	uns_s1 = (unsigned char *)s1;
	uns_s2 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (uns_s1[i] != '\0' && uns_s2[i] != '\0'
		&& uns_s1[i] == uns_s2[i] && i < n - 1)
		i++;
	return (uns_s1[i] - uns_s2[i]);
}
