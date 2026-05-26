NAME = minishell
CC	= cc
CFLAGS	= -Wall -Wextra -Werror -I.
RM	= rm -f

RESET   = \033[0m
RED     = \033[0;31m
GREEN   = \033[0;32m
CYAN    = \033[0;36m
YELLOW  = \033[0;33m
BLUE    = \033[0;34m
PURPLE  = \033[0;35m

SRCS =  main.c \
	    parsing/ini_token_1.c \
		parsing/ini_token_2.c \
		parsing/ini_env.c \
		parsing/ini_cmd.c \
		parsing/syntax_checker.c \
		parsing/expansion_helper.c \
		helper_functions/tool_box_children.c \
		helper_functions/tool_box_fd.c \
		helper_functions/tool_box_heredoc.c \
		helper_functions/tool_box_main.c \
		helper_functions/tool_box_parsing.c \
		helper_functions/tool_box_exec.c \
		helper_functions/tool_box_free.c \
		helper_functions/tool_box_env.c \
		helper_functions/tool_box_env2.c \
		helper_functions/tool_box_sig.c \
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
		@make bonus -C libft --no-print-directory > /dev/null 2>&1
		@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
		@echo -e "$(RED)███╗   ███╗██╗███╗   ██╗██╗███████╗███████╗██╗  ██╗███████╗██╗     ██╗     "
		@echo -e "$(YELLOW)████╗ ████║██║████╗  ██║██║██╔════╝██╔════╝██║  ██║██╔════╝██║     ██║     "
		@echo -e "$(GREEN)██╔████╔██║██║██╔██╗ ██║██║███████╗███████╗███████║█████╗  ██║     ██║     "
		@echo -e "$(CYAN)██║╚██╔╝██║██║██║╚██╗██║██║╚════██║╚════██║██╔══██║██╔══╝  ██║     ██║     "
		@echo -e "$(BLUE)██║ ╚═╝ ██║██║██║ ╚████║██║███████║███████║██║  ██║███████╗███████╗███████╗"
		@echo -e "$(PURPLE)╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@make -C libft clean --no-print-directory > /dev/null 2>&1

fclean: clean
	@$(RM) $(NAME)
	@make -C libft fclean --no-print-directory > /dev/null 2>&1

re: fclean all

.PHONY: all clean fclean re