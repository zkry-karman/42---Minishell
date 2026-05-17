NAME = minishell
CC	= cc
CFLAGS	= -Wall -Wextra -Werror -I.
RM	= rm -f

SRCS =  main.c \
	    parsing/ini_token_1.c \
		parsing/ini_token_2.c \
		parsing/ini_env.c \
		parsing/ini_cmd.c \
		parsing/syntax_checker.c\
		helper_functions/tool_box_fd.c \
		helper_functions/tool_box_heredoc.c \
		helper_functions/tool_box_parsing.c \
		helper_functions/tool_box_exec.c \
		helper_functions/tool_box_free.c \
		helper_functions/tool_box_env.c \
		execution/execution.c \
		execution/exits.c \
		execution/fd_management.c \
		execution/heredoc_management.c \
		execution/loop_commands.c \
		built_ins/built_in_cmds.c \
		built_ins/cd.c \
		built_ins/echo.c \
		built_ins/env.c \
		built_ins/exit.c \
		built_ins/export.c \
		built_ins/pwd.c \
		built_ins/unset.c \

OBJS = $(SRCS:.c=.o)

LIBFT = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS)
		@make bonus -C libft
		$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)
	make -C libft clean

fclean: clean
	$(RM) $(NAME)
	make -C libft fclean

re: fclean all

.PHONY: all clean fclean re