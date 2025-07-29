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

void	init_mini_data(t_minishell *minishell, char **envp)
{
	minishell->input = NULL;
	minishell->tokens = NULL;
	minishell->command_list = NULL;
	minishell->temp_pids = NULL;
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
	minishell->tokens = tokenizer(minishell->input);
	expand_tokens(minishell, minishell->tokens, minishell->env_list);
	minishell->command_list = parse_command(minishell, &minishell->tokens);
	execute_pipeline(minishell->command_list, minishell);
	free_loop(minishell);
}
