/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:42:57 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/09 16:28:39 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int lexer(t_token *tokens)
// {
//     // t_token *curr = tokens;

//     // while (curr && curr->type != TOKEN_EOF)
//     // {
//     //     if (curr->type == TOKEN_PIPE)
//     //     {
//     //         if (!curr->next || curr->next->type == TOKEN_PIPE)
//     //             return error_msg("syntax error near unexpected token `|`");
//     //     }
//     //     else if (curr->type == TOKEN_REDIRECT_IN || curr->type == TOKEN_REDIRECT_OUT ||
//     //              curr->type == TOKEN_APPEND || curr->type == TOKEN_HEREDOC)
//     //     {
//     //         if (!curr->next || curr->next->type != TOKEN_WORD)
//     //             return error_msg("syntax error near redirect");
//     //     }
//     //     curr = curr->next;
//     // }
//     // return 0;
// }