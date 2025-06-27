/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 01:02:18 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/23 01:02:18 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_token_list(t_token *head)
{
    t_token *tmp;
    int count = 0;

    while (head)
    {
        tmp = head->next;
        free(head->value);
        free(head);
        head = tmp;
        count++;
    }
}
