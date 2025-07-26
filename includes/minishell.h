/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azorlutu <azorlutu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 17:50:44 by azorlutu          #+#    #+#             */
/*   Updated: 2025/07/26 14:52:45 by azorlutu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>  //stat(), fstat(), lstat()
# include <dirent.h>	   //opendir(), readdir(), closedir()
# include <string.h>	   //strerror()
# include <fcntl.h>	   //open(), O_RDONLY
# include <sys/ioctl.h> //ioctl()
# include <signal.h>	   //signal()
# include <sys/wait.h>  //waitpid()
# include <errno.h>	   //errno, perror()

# include "../libft/libft.h"

extern int	g_signal_flag;

//* ************************************* */
//* Enums
//* ************************************* */
// Word (Example: "hello", "world")
// <
// >
// <<
// >>
// |
// Last token (to end the list)
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_PIPE,
	TOKEN_EOF
}	t_token_type;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote_type;

//* ************************************* */
//* Structures
//* ************************************* */

typedef struct s_token
{
	t_token_type	type;
	t_quote_type	quote;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redirection
{
	t_token_type			type;
	char					*filename;
	t_quote_type			redir_quote;
	char					*delimiter_raw;
	char					*delimiter_expanded;
	int						fd;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	char				**args;
	t_redirection		*redirects;
	struct s_command	*next_pipe;
	int					is_quoted_empty_command;
}	t_command;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_minishell
{
	char		*input;
	t_env		*env_list;
	t_token		*tokens;
	t_command	*command_list;
	char		**env_array;
	int			last_exit_code;
	int			has_syntax_error;
}	t_minishell;

/* Expand variable fonksiyonu norm structları */

typedef struct s_expand_ctx
{
	char			*token_value;
	int				*i;
	t_quote_type	quote_type;
}	t_expand_ctx;

/*	Minishell */
void			init_mini_data(t_minishell *minishell, char **envp);
void			init_minishell(t_minishell *minishell);

/* Free */
void			free_minishell(t_minishell *minishell);
void			free_loop(t_minishell *minishell);

/*	Signal */
void			setup_interactive_signals(void);
void			setup_heredoc_signals(void);
void			setup_default_signals(void);

/* Builtins */
int				is_builtin(char *cmd);
int				run_builtin(t_command *cmd, t_minishell *minishell);
int				is_valid_key(const char *key);
int				builtin_echo(char **args);
int				builtin_cd(char **args, t_minishell *minishell);
int				builtin_pwd(void);
int				builtin_export(char **args, t_minishell *minishell);
int				builtin_unset(char **args, t_minishell *minishell);
int				builtin_env(char **args, t_minishell *minishell);
int				builtin_exit(char **args, t_minishell *minishell);
char			*get_home_path(t_env *env_list);
char			*get_oldpwd_path(t_env *env_list);
void			print_cd_error(char *arg);
int				cd_too_many_args(void);
char			*get_cd_target(char *arg, t_minishell *minishell);
void			print_env_sorted(t_env *env_list);
int				split_key_value(char *arg, char **key, char **value);
int				print_export_error(char *arg);
int				handle_no_args(t_minishell *minishell);
void			set_env_and_update(t_minishell *minishell,
					char *key, char *value);
void			no_args_exit(t_minishell *minishell);
void			non_numeric_exit(char *arg, t_minishell *minishell);
void			too_many_args_exit(t_minishell *minishell);
void			normal_exit(char *arg, t_minishell *minishell);

/*	Environments	*/
t_env			*init_env_list(char **envp);
char			*get_env_value(t_env *env_list, const char *key);
void			set_env_value(t_env **env_list,
					const char *key, const char *value);
void			unset_env_value(t_env **env_list, const char *key);
char			**env_list_to_array(t_env *env_list);
void			free_env_array(char **env_array);
void			update_env_array(t_minishell *mini);
void			free_env_list(t_env *env_list);
void			free_env_array(char **env_array);
t_env			*sort_env_list(t_env *env);
void			update_existing_key(t_env *node, const char *value);

/*      Tokenizer      */
t_token			*tokenizer(char *input, t_minishell *minishell);
t_token			*create_token(t_token_type type,
					char *value, t_quote_type quote);
void			add_token(t_token **head, t_token *new_token);
void			free_token_list(t_token *head);
int				is_whitespace(char c);
int				is_operator(char c);
char			*get_word(const char *input, int *i);
t_token_type	get_operator_type(const char *input);
int				get_token_len(t_token_type type);
void			determine_quote_type(char c, t_quote_type *quote_out);
int				measure_quoted_length(const char *input,
					int *i, char quote_char);
void			skip_whitespace(char *input, int *i);
char			*get_combined_token(const char *input,
					int *i, t_quote_type *quote_out);

/*  Expander  */
char			*ft_strjoin_free(char *s1, char *s2);
void			expand_tokens(t_minishell *minishell,
					t_token *token_list, t_env *env_list);
void			expand_exit_status(t_minishell *minishell,
					char **result, int *i);
char			*get_variable_value(t_expand_ctx *ctx, t_env *env_list);
char			*expand_word(t_minishell *minishell,
					t_quote_type quote_type,
					char *token_value, t_env *env_list);
int				is_heredoc_delimiter(t_token *prev);
int				expand_token_value(t_minishell *minishell,
					t_token *cursor, t_env *env_list);

/*  Parser  */
t_command		*parse_command(t_minishell *minishell, t_token **tokens);
t_command		*init_command(void);
void			handle_pipe_recursively(t_minishell *minishell,
					t_token **cursor, t_command *cmd);
char			**list_to_array(t_list *args);
int				add_arg_to_list(t_list **args, char *value);
t_command		*init_command(void);

/* Executor */
char			*ft_strjoin_three(const char *s1,
					const char *s2, const char *s3);
void			free_split(char **split);
void			execute_pipeline(t_command *cmd, t_minishell *minishell);
char			*resolve_path(const char *cmd, t_env *env_list);
int				set_redirection_fds(t_redirection *redir);
int				create_empty_redirect_files(t_redirection *redir);
int				handle_heredoc(t_command *cmd, t_minishell *minishell);
int				pipe_safe(int pipe_fd[2]);
pid_t			fork_safe(void);
void			print_and_exit(t_minishell *minishell,
					char *prefix, char *msg, int code);
void			check_and_execute(t_command *cmd, t_minishell *minishell);
int				empty_or_null_command(t_command *cmd, t_minishell *minishell);
int				run_parent_builtin_if_needed(t_command *cmd,
					t_minishell *minishell);
void			execute_pipeline_fork(t_command *cmd, t_minishell *minishell);
int				process_pipeline_command(t_command *cmd,
					t_minishell *minishell, int *prev_fd);
void			finalize_pipeline_status(int status, t_minishell *minishell);
char			*heredoc_read_loop(t_minishell *minishell,
					t_redirection *redir);
int				run_heredoc_child(t_minishell *minishell,
					t_redirection *redir, int write_fd);
void			handle_heredoc_child(t_minishell *minishell,
					t_redirection *redir, int *pipefd);

void			free_redirections(t_redirection *redir);
void			free_command_list(t_command *cmd);
void			free_arg_list(t_list *args);
int				restore_fds_and_return(int in_bak, int out_bak,
					int ret, t_minishell *minishell);
int				is_quoted_empty_command_error(t_command *cmd,
					t_minishell *minishell);
int				handle_redirect_only_command(t_command *cmd,
					t_minishell *minishell);
int				is_only_operator_syntax_error(t_minishell *minishell);

#endif