/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 22:18:05 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/19 22:18:05 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void add_token(t_token **head, t_token *new_token)
{
    t_token *current;

    if(!head || !new_token)
        return ;
    if(!*head)
    {
        *head = new_token;
        return ;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_token;
}

t_token	*create_token(t_token_type type, char *value, t_quote_type quote)
{
    t_token	*new_token;

    new_token = (t_token *)malloc(sizeof(t_token));
    if (!new_token)
        return (NULL);
    new_token->type = type;
    new_token->value = value;
    new_token->quote = quote;
    new_token->next = NULL;
    return (new_token);
}