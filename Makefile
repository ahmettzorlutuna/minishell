# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: azorlutu <azorlutu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/17 17:59:39 by azorlutu          #+#    #+#              #
#    Updated: 2025/07/26 14:51:29 by azorlutu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRC_PATH = ./srcs/
INC_PATH = ./includes/
OBJ_PATH = ./objs/

SRC	=	builtins/utils/builtin_utils.c \
		builtins/cd/builtin_cd.c \
		builtins/cd/builtin_cd_utils.c \
		builtins/echo/builtin_echo.c \
		builtins/env/builtin_env.c \
		builtins/exit/builtin_exit.c \
		builtins/exit/builtin_exit_utils.c \
		builtins/export/builtin_export.c \
		builtins/export/builtin_export_utils.c \
		builtins/pwd/builtin_pwd.c \
		builtins/unset/builtin_unset.c \
		env/env_array.c \
		env/env_list.c \
		env/free_env.c \
		env/sort_env_list.c \
		env/env_list_utils.c \
		executor/executor_utils4.c \
		executor/executor_utils3.c \
		executor/executor_utils2.c \
		executor/executor_utils.c \
		executor/executor.c \
		executor/heredoc.c \
		executor/heredoc_utils.c \
		executor/redirection.c \
		expander/expander.c \
		expander/expander_utils.c \
		tokenizer/free_tokenizer.c \
		tokenizer/get_combined_token.c \
		tokenizer/token_list.c \
		tokenizer/token_utils.c \
		tokenizer/token_utils2.c \
		tokenizer/tokenizer.c \
		main/main.c \
		main/minishell.c \
		parser/parser.c \
		parser/parser_utils.c \
		parser/parser_node_utils.c \
		utils/handle_signals.c \
		utils/free_loop.c

SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ 	= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))
INC		= -I $(INC_PATH) -I $(LIBFT_PATH)

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

all: $(OBJ_PATH) $(LIBFT) $(NAME)

$(OBJ_PATH):
	mkdir -p $(OBJ_PATH)
	mkdir -p $(OBJ_PATH)/builtins
	mkdir -p $(OBJ_PATH)/builtins/cd
	mkdir -p $(OBJ_PATH)/builtins/echo
	mkdir -p $(OBJ_PATH)/builtins/env
	mkdir -p $(OBJ_PATH)/builtins/exit
	mkdir -p $(OBJ_PATH)/builtins/export
	mkdir -p $(OBJ_PATH)/builtins/pwd
	mkdir -p $(OBJ_PATH)/builtins/unset
	mkdir -p $(OBJ_PATH)/builtins/utils
	mkdir -p $(OBJ_PATH)/env
	mkdir -p $(OBJ_PATH)/executor
	mkdir -p $(OBJ_PATH)/expander
	mkdir -p $(OBJ_PATH)/main
	mkdir -p $(OBJ_PATH)/parser
	mkdir -p $(OBJ_PATH)/tokenizer
	mkdir -p $(OBJ_PATH)/test
	mkdir -p $(OBJ_PATH)/lexer
	mkdir -p $(OBJ_PATH)/utils

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
	@echo "Compiling $< to $@"

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INC) $(LIBFT) -l readline

$(LIBFT):
	make -C $(LIBFT_PATH)

clean:
	rm -rf $(OBJ_PATH)
	make -C $(LIBFT_PATH) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all re clean fclean