NAME = minishell
CC	= cc
CFLAGS	= -Wall -Wextra -Werror
RM	= rm -f

SRCS =  main.c \
	    ini_token_1.c \
		ini_token_2.c \
		ini_cmd.c \
		tool_box_parsing.c \
		tool_box_execution.c \
        execution.c \
        exit_program.c \
	
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