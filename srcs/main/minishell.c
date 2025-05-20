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

void	exit_minishell(t_minishell *minishell)
{
	free_env_list(minishell->env_list);
	free_env_array(minishell->env_array);
	free(minishell->input);
	free(minishell);
	free(minishell->tokens);
	exit(0);
}

void	init_mini_data(t_minishell *minishell, char **envp)
{
	minishell->input = NULL;
	minishell->tokens = NULL;
	minishell->env_list = init_env_list(envp);
	minishell->env_array = env_list_to_array(minishell->env_list);
	minishell->last_exit_code = 0;
	minishell->number_of_prompts = 0;
}

void init_minishell(t_minishell *minishell)
{
	add_history(minishell->input);
	minishell->tokens = tokenizer(minishell->input);
}
