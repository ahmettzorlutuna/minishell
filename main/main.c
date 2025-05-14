/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:49:57 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/04 17:49:58 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = strdup(value);
	token->next = NULL;
	return (token);
}

static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static t_token_type	get_operator_type(const char *str, int *len)
{
	if (strncmp(str, ">>", 2) == 0)
		return (*len = 2, T_APPEND);
	if (strncmp(str, "<<", 2) == 0)
		return (*len = 2, T_HEREDOC);
	if (*str == '>')
		return (*len = 1, T_REDIRECT_OUT);
	if (*str == '<')
		return (*len = 1, T_REDIRECT_IN);
	if (*str == '|')
		return (*len = 1, T_PIPE);
	return (*len = 0, T_EOF);
}

t_token	*lexer(const char *input)
{
	t_token	*head = NULL;
	t_token	**current = &head;
	int		i = 0, len;

	while (input[i])
	{
		while (isspace(input[i]))
			i++;
		if (is_operator(input[i]))
		{
			t_token_type type = get_operator_type(&input[i], &len);
			char *op = strndup(&input[i], len);
			*current = create_token(type, op);
			free(op);
			i += len;
		}
		else
		{
			int start = i;
			while (input[i] && !isspace(input[i]) && !is_operator(input[i]))
				i++;
			char *word = strndup(&input[start], i - start);
			*current = create_token(T_WORD, word);
			free(word);
		}
		if (*current)
			current = &(*current)->next;
	}
	return (head);
}

void	print_tokens(t_token *token)
{
	while (token)
	{
		printf("TOKEN: %-15s VALUE: %s\n", 
			(token->type == T_WORD) ? "WORD" :
			(token->type == T_PIPE) ? "PIPE" :
            (token->type == T_VAR) ? "VAR" :
			(token->type == T_REDIRECT_IN) ? "REDIRECT_IN" :
			(token->type == T_REDIRECT_OUT) ? "REDIRECT_OUT" :
			(token->type == T_HEREDOC) ? "HEREDOC" :
			(token->type == T_APPEND) ? "APPEND" : "UNKNOWN",
			token->value);
		token = token->next;
	}
}

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    (void)envp;
    
	char *rl;

    while(1)
    {
        rl = readline("minishell> ");
        t_token *tokens = lexer(rl);
        if (rl == NULL)
        {
            printf("exit\n");
            break;
        }
        if (*rl)
            add_history(rl);
        print_tokens(tokens);
        free(rl);
    }

    return 0;
}
