/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul.com  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 21:44:48 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/19 21:44:49 by azorlutu         ###   ########.tr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

static int is_operator(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

static char *get_word(const char *input, int *i)
{
    int start;
    int len;

    start = *i;
    len = 0;
    while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]))
    {
        (*i)++;
        len++;
    }
    return (ft_substr(input, start, len));
}

static t_token_type get_operator_type(const char *input, int *i)
{
    if(input[*i] == '<' && input[*i + 1] == '<')
    {
        (*i)++;
        return (TOKEN_HEREDOC);
    }
    if(input[*i] == '>' && input[*i + 1] == '>')
    {
        (*i)++;
        return (TOKEN_APPEND);
    }
    if(input[*i] == '<')
        return(TOKEN_REDIRECT_IN);
    if(input[*i] == '>')
        return(TOKEN_REDIRECT_OUT);
    if(input[*i] == '|')
        return(TOKEN_PIPE);
    return (TOKEN_EOF);
}

static int get_token_len(t_token_type type)
{
    if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT || type == TOKEN_PIPE)
        return (1);
    if (type == TOKEN_HEREDOC || type == TOKEN_APPEND)
        return (2);
    return (0);
}


t_token *tokenizer(char *input)
{
    t_token *token_list;
    t_token *new_token;
    char *word;
    int i;

    token_list = NULL;
    i = 0;
    while (input[i])
    {
        if(is_whitespace(input[i]))
            i++;
        else if (is_operator(input[i]))
        {
            t_token_type type = get_operator_type(input, &i);
            new_token = create_token(type, ft_substr(input, i++, get_token_len(type)));
            if (!new_token)
                return (NULL);
            add_token(&token_list, new_token);
        }
        else
        {
            word = get_word(input, &i);
            if (!word)
                return (NULL);
            new_token = create_token(TOKEN_WORD, word);
            if (!new_token)
                return (NULL);
            add_token(&token_list, new_token);
        }
    }
    add_token(&token_list, create_token(TOKEN_EOF, NULL));
    return (token_list);
}