/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:11:21 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/18 14:11:22 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*uns_s1;
	unsigned char	*uns_s2;
	size_t			i;

	uns_s1 = (unsigned char *)s1;
	uns_s2 = (unsigned char *)s2;
	i = 0;
	while (uns_s1[i] != '\0' && uns_s2[i] != '\0' && uns_s1[i] == uns_s2[i])
		i++;
	return (uns_s1[i] - uns_s2[i]);
}
