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
# include <sys/stat.h>  //stat(), fstat(), lstat()
# include <dirent.h>	   //opendir(), readdir(), closedir()
# include <string.h>	   //strerror()
# include <fcntl.h>	   //open(), O_RDONLY
# include <sys/ioctl.h> //ioctl()
# include <signal.h>	   //signal()

# include <curses.h> // for tgetent(), tgetstr(), tputs()
# include <term.h>	// for tgetent(), tgetstr(), tputs()

# include "../libft/libft.h"

extern	volatile	sig_atomic_t g_signal_flag;

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

typedef struct s_token
{
	t_token_type type;	  // Token türü (örneğin: TOKEN_WORD, TOKEN_REDIRECT_IN, vb.)
	t_quote_type quote; // Alıntı türü (örneğin: NO_QUOTE, SINGLE_QUOTE, DOUBLE_QUOTE)
	char *value;		  // Tokenin değeri (örneğin: "hello", ">", "<", "|", vb.)
	struct s_token *next; // Sonraki tokeni işaret eder
} t_token;

typedef struct s_redir
{
	t_token_type type;	  // Yönlendirme türü (in, out, append, vb.)
	char *file;			  // Yönlendirilen dosya adı
	struct s_redir *next; // Birden fazla yönlendirme olabilir
} t_redirection;

typedef	struct	s_cmd
{
	char *cmd;				 // Komut adı
	char **args;			 // Komutun argümanları (null-terminated)
	t_redirection *redir;	 // Yönlendirmeler
	char *heredoc_content;	 // Heredoc içeriği (veya geçici dosya adı)
	char *heredoc_delimiter; // Heredoc sınırlayıcısı
	int pipe_in_fd;			 // Önceki komuttan okuma için pipe FD
	int pipe_out_fd;		 // Sonraki komuta yazma için pipe FD
	struct s_cmd *next;		 // Boru hattındaki sonraki komut
} t_command;

typedef	struct	s_env
{
	char *key;	 // Örn: "PATH"
	char *value; // Örn: "/usr/bin:/bin"
	struct s_env *next;
} t_env;

typedef	struct	s_minishell
{
	char *input;		   // Kullanıcıdan alınan girdi
	t_env *env_list;	   // Çevresel değişkenler (linked list)
	t_token *tokens;	   // Tokenler (linked list)
	char **env_array;	   // Çevresel değişkenler (array hali execve çalıştırmak için dizi formatında vercez)
	int last_exit_code;	   // Son çıkış kodu $?
	int number_of_prompts; // Kaç tane prompt gösterildi
} t_minishell;

/*	Minishell */
void	init_mini_data(t_minishell *minishell, char **envp);
void	init_minishell(t_minishell *minishell);

/*	signal */
void	init_signal_handler(void);

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

/*      Tokenizer      */
t_token			*tokenizer(char *input);
t_token			*create_token(t_token_type type, char *value);
void			add_token(t_token **head, t_token *new_token);
void			free_token_list(t_token *head);
int				is_whitespace(char c);
int				is_operator(char c);
char			*get_word(const char *input, int *i);
t_token_type	get_operator_type(const char *input);
int				get_token_len(t_token_type type);

/*  Expander  */
char *ft_strjoin_free(char *s1, char *s2);
void expand_tokens(t_token *token_list, t_env *env_list);

/*	Test functions	*/
void	print_tokens(t_token *tokens);

#endif