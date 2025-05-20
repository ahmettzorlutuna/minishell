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

static char *get_word_with_quotes(const char *input, int *i)
{
    int start, len;
    char quote;
    char *quoted_part;
    char *rest_part;
    char *final_word;

    len = 0;
    quote = input[*i];

    (*i)++;             // açılış tırnağını geç
    start = *i;

    while (input[*i] && input[*i] != quote)
    {
        (*i)++;
        len++;
    }

    if (!input[*i])
    {
        ft_putstr_fd("syntax error: unexpected EOF while looking for matching quote\n", 2);
        return (NULL);
    }

    quoted_part = ft_substr(input, start, len); // örn: ahmet

    (*i)++; // kapanış tırnağını geç

    // Şimdi kalan kısmı oku: sadece quote yoksa ve operator değilse
    start = *i;
    len = 0;
    while (input[*i] && !is_whitespace(input[*i]) && !is_operator(input[*i]) && input[*i] != '\'' && input[*i] != '"')
    {
        (*i)++;
        len++;
    }
    rest_part = ft_substr(input, start, len); // örn: 123

    // İki parçayı birleştir
    final_word = ft_strjoin(quoted_part, rest_part);

    free(quoted_part);
    free(rest_part);

    return (final_word);
}

static t_token_type get_operator_type(const char *input, int *i)
{
    if (input[*i] == '<' && input[*i + 1] == '<')
        return (TOKEN_HEREDOC);
    if (input[*i] == '>' && input[*i + 1] == '>')
        return (TOKEN_APPEND);
    if (input[*i] == '<')
        return (TOKEN_REDIRECT_IN);
    if (input[*i] == '>')
        return (TOKEN_REDIRECT_OUT);
    if (input[*i] == '|')
        return (TOKEN_PIPE);
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
        if (is_whitespace(input[i]))
            i++;
        else if (is_operator(input[i]))
        {
            t_token_type type = get_operator_type(&input[i], &i);
            int len = get_token_len(type);
            new_token = create_token(type, ft_substr(input, i, len));
            if (!new_token)
                return (NULL);
            add_token(&token_list, new_token);
            i += len;
        }
        else if(input[i] == '\'' || input[i] == '\"')
        {
            word = get_word_with_quotes(input, &i);
            if (!word)
                return (NULL);
            new_token = create_token(TOKEN_WORD, word);
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