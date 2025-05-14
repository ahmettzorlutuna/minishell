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
# include <sys/stat.h> //stat(), fstat(), lstat()
# include <dirent.h> //opendir(), readdir(), closedir()
# include <string.h> //strerror()
# include <fcntl.h> //open(), O_RDONLY
# include <sys/ioctl.h> //ioctl()

#include <curses.h> // for tgetent(), tgetstr(), tputs()
#include <term.h> // for tgetent(), tgetstr(), tputs()

//* ************************************************************************** */
//* Structures
//* ************************************************************************** */

typedef struct s_minishell
{
    char **envp; // Çevresel değişkenler
    char *input; // Kullanıcıdan alınan girdi
	int last_exit_code; // Son çıkış kodu $?
} t_minishell;

typedef struct s_token
{
	t_token_type	type; // Token türü (örneğin: TOKEN_WORD, TOKEN_REDIRECT_IN, vb.)
	char			*value; // Tokenin değeri (örneğin: "hello", ">", "<", "|", vb.)
	struct s_token	*next; // Sonraki tokeni işaret eder
    struct s_token	*prev; // Önceki tokeni işaret eder
}	t_token;

typedef struct s_redirection
{
    t_token_type type;  // Yönlendirme türü (in, out, append, vb.)
    char *file;         // Yönlendirilen dosya adı
    struct s_redirection *next; // Birden fazla yönlendirme olabilir
} t_redirection;

typedef struct s_command
{
    char *cmd;             // Komut adı (örneğin: echo, pwd, cd, vb.)
    char **args;           // Komutun argümanları (örneğin: ["-n", "Hello"])
    t_redirection *redir;  // Yönlendirmeler (input, output, append, vb.)
    struct s_command *next; // Birden fazla komut için linked list
} t_command;

typedef struct s_env
{
	char	*key;   // Örn: "PATH"
	char	*value; // Örn: "/usr/bin:/bin"
	struct s_env *next;
}	t_env;


//* ************************************************************************** */
//* Enums
//* ************************************************************************** */
typedef enum e_token_type
{
	TOKEN_WORD,         // ahmet, foo, "merhaba"
    TOKEN_CMD,          // Komutlar (örneğin: echo, cd, pwd)
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_HEREDOC,      // <<
	TOKEN_APPEND,       // >>
	TOKEN_PIPE,         // |
	TOKEN_EOF           // Son token (listeyi sonlandırmak için)
}	t_token_type;

typedef enum e_quote_type
{
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
}	t_quote_type;

#endif