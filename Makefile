NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = main.c clean.c error.c env.c env_ops.c executor.c flatten.c pipe.c command.c path.c 
OBJS = $(SRCS:.c=.o)

MAKE = make -C
LIBFT_PATH = libft
LIBFT = -L $(LIBFT_PATH) -lft
READLINE = -lreadline

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) $(LIBFT_PATH) all
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(READLINE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) $(LIBFT_PATH) clean
	rm -f $(OBJS)

fclean: clean
	$(MAKE) $(LIBFT_PATH) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
