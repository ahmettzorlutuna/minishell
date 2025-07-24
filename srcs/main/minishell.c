/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:30:54 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/18 13:30:54 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_token_parts(t_token *token)
{
	t_token_part	*part;

	if (!token)
	{
		printf("Token is NULL\n");
		return ;
	}
	printf("Token Type: %d\n", token->type);
	part = token->parts;
	while (part)
	{
		printf("  Part: \"%s\"", part->str);
		if (part->quote == SINGLE_QUOTE)
			printf(" (SINGLE_QUOTE)\n");
		else if (part->quote == DOUBLE_QUOTE)
			printf(" (DOUBLE_QUOTE)\n");
		else
			printf(" (NO_QUOTE)\n");
		part = part->next;
	}
}

void	print_token_list(t_token *token_list)
{
	int token_id = 0;

	while (token_list)
	{
		printf("TOKEN #%d:\n", token_id++);
		printf("  Type: %d\n", token_list->type);
		t_token_part *part = token_list->parts;
		while (part)
		{
			printf("    Part: \"%s\" (%s)\n", part->str,
				part->quote == SINGLE_QUOTE ? "SINGLE" :
				part->quote == DOUBLE_QUOTE ? "DOUBLE" : "NO_QUOTE");
			part = part->next;
		}
		printf("  Final value: %s\n", token_list->value ? token_list->value : "(null)");
		token_list = token_list->next;
	}
}

void	print_args(char **args)
{
	int	i;

	if (!args)
	{
		printf("  (args = NULL)\n");
		return ;
	}
	i = 0;
	while (args[i])
	{
		printf("  Arg[%d]: %s\n", i, args[i]);
		i++;
	}
}

void	init_mini_data(t_minishell *minishell, char **envp)
{
	minishell->input = NULL;
	minishell->tokens = NULL;
	minishell->command_list = NULL;
	minishell->last_exit_code = 0;
    minishell->has_syntax_error = 0;
	minishell->env_list = init_env_list(envp);
	minishell->env_array = env_list_to_array(minishell->env_list);
}

void	init_minishell(t_minishell *minishell)
{
    minishell->has_syntax_error = 0;
	add_history(minishell->input);
	free_token_list(minishell->tokens);
	minishell->tokens = tokenizer(minishell->input, minishell);
    // t_token *tmp = minishell->tokens;
    // while (tmp)
    // {
    //     printf("TOKEN:\n");
    //     print_token_parts(tmp); // part->str + part->quote
    //     tmp = tmp->next;
    // }
    print_token_list(minishell->tokens);
	expand_tokens(minishell, minishell->tokens, minishell->env_list);
	minishell->command_list = parse_command(minishell, &minishell->tokens);
    // t_command *cmd = minishell->command_list;
    // int index = 0;
    // while (cmd)
    // {
    //     printf("COMMAND #%d:\n", index++);
    //     print_args(cmd->args); // kendi fonksiyonun
    //     cmd = cmd->next_pipe;
    // }
	execute_pipeline(minishell->command_list, minishell);
	free_loop(minishell);
}
