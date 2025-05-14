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

typedef struct s_minishell
{
    char **envp;
    char *input;
    char *prompt;
} t_minishell;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
    struct s_token	*prev;
}	t_token;


//* ************************************************************************** */
//* Enums
//* ************************************************************************** */
typedef enum e_token_type
{
	T_WORD,         // ls, -la, txt gibi normal kelimeler
    T_VAR,          // $PATH, $USER gibi değişkenler
	T_PIPE,         // |
	T_REDIRECT_IN,  // <
	T_REDIRECT_OUT, // >
	T_HEREDOC,      // <<
	T_APPEND,       // >>
	T_EOF           // Son token (listeyi sonlandırmak için)
}	t_token_type;

typedef enum e_quote_type
{
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
}	t_quote_type;

#endif