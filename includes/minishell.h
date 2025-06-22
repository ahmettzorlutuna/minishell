/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42istanbul      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:50:44 by azorlutu          #+#    #+#             */
/*   Updated: 2025/05/04 17:50:45 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>		//stat(), fstat(), lstat()
# include <dirent.h>		//opendir(), readdir(), closedir()
# include <string.h>		//strerror()
# include <fcntl.h>			//open(), O_RDONLY
# include <sys/ioctl.h>		//ioctl()
# include <signal.h>		//signal()
# include <sys/wait.h>		//waitpid()
# include <errno.h>			//errno, perror()

# include <curses.h>		// for tgetent(), tgetstr(), tputs()
# include <term.h>			// for tgetent(), tgetstr(), tputs()

# include "../libft/libft.h"

extern	int g_signal_flag;

//* ************************************************************************** */
//* Enums
//* ************************************************************************** */
typedef enum e_token_type
{
	TOKEN_WORD,			// Kelime (örneğin: "hello", "world")
	TOKEN_REDIRECT_IN,	// <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_HEREDOC,		// <<
	TOKEN_APPEND,		// >>
	TOKEN_PIPE,			// |
	TOKEN_EOF			// Son token (listeyi sonlandırmak için)
} t_token_type;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
} t_quote_type;

//* ************************************************************************** */
//* Structures
//* ************************************************************************** */

/*
TOKENİZER
Bir kelimeyi/parçayı analiz edip, "hangi quote içinde mi?", "kelime ne?" gibi bilgileri geçici olarak tutar.
Kelimeyi (ve varsa tırnak bilgisini) doğru şekilde çıkarıp geri döndürmek için
*/
typedef struct s_word_info
{
	char *value;
	t_quote_type quote;
} t_word_info;

typedef struct s_token
{
	t_token_type type;
	t_quote_type quote;
	char *value;
	struct s_token *next;
} t_token;

typedef struct s_redirection
{
	t_token_type type;
	char *filename;
	int fd;
	struct s_redirection *next;
} t_redirection;

typedef	struct	s_command
{
	char **args;
	t_redirection *redirects;
	struct s_command	*next_pipe;
} t_command;

typedef	struct	s_env
{
	char *key;
	char *value;
	struct s_env *next;
} t_env;

typedef	struct	s_minishell
{
	char *input;
	t_env *env_list;
	t_token *tokens;
	t_command *command_list;
	char **env_array;
	int last_exit_code;
} t_minishell;

/*	Minishell */
void	init_mini_data(t_minishell *minishell, char **envp);
void	init_minishell(t_minishell *minishell);

/*	Signal */
void	setup_interactive_signals(void);
void	setup_heredoc_signals(void);
void	setup_default_signals(void);

/* Builtins */
int	is_parent_builtin(char *cmd);
int	is_builtin(char *cmd);
int	run_builtin(t_command *cmd, t_minishell *minishell);
int	is_valid_key(const char *key);
int	builtin_echo(char **args);
int	builtin_cd(char **args, t_minishell *minishell);
int	builtin_pwd(void);
int	builtin_export(char **args, t_minishell *minishell);
int	builtin_unset(char **args, t_minishell *minishell);
int	builtin_env(char **args, t_minishell *minishell);
int	builtin_exit(char **args, t_minishell *minishell);

/*	Environments	*/
t_env	*init_env_list(char **envp);
char	*get_env_value(t_env *env_list, const char *key);
void 	set_env_value(t_env **env_list, const char *key, const char *value);
void	unset_env_value(t_env **env_list, const char *key);
char	**env_list_to_array(t_env *env_list);
void	free_env_array(char **env_array);
void	update_env_array(t_minishell *mini);
void	free_env_list(t_env *env_list);
void	free_env_array(char **env_array);
void	exit_minishell(t_minishell *minishell);
t_env	*sort_env_list(t_env *env);

/*      Tokenizer      */
t_token			*tokenizer(char *input);
t_token	*create_token(t_token_type type, char *value, t_quote_type quote);
void			add_token(t_token **head, t_token *new_token);
void			free_token_list(t_token *head);
int				is_whitespace(char c);
int				is_operator(char c);
char			*get_word(const char *input, int *i);
t_token_type	get_operator_type(const char *input);
int				get_token_len(t_token_type type);

/*  Expander  */
char *ft_strjoin_free(char *s1, char *s2);
void expand_tokens(t_minishell *minishell, t_token *token_list, t_env *env_list);

/*  Parser  */
t_command *parse_command(t_token **tokens);

/* Executor */
char *ft_strjoin_three(const char *s1, const char *s2, const char *s3);
void free_split(char **split);
void execute_pipeline(t_command *cmd, t_minishell *minishell);
char *resolve_path(const char *cmd, t_env *env_list);
int set_redirection_fds(t_redirection *redir);
int handle_heredoc(t_command *cmd);
int pipe_safe(int pipe_fd[2]);
pid_t fork_safe(void);
void	print_and_exit(char *prefix, char *msg, int code);
void	check_and_execute(t_command *cmd, t_minishell *minishell);

/*	Test functions	*/
void	print_tokens(t_token *tokens);
void	print_parsed_command(t_command *cmd);
void	print_command(t_command *cmd);

#endif