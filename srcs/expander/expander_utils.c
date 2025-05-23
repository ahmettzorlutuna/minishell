/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 19:01:35 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/23 19:01:38 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *ft_strjoin_free(char *s1, char *s2)
{
	char *new;
	new = ft_strjoin(s1, s2);
	free(s1);
	return (new);
}
