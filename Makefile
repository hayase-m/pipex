NAME = pipex

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
INC = -Iinclude -I$(LIBFT_DIR)

SRCS = src/main.c src/pipex_utils.c src/pipex_process.c src/pipex_error.c

OBJS = $(SRCS:.c=.o)
LINK_FLAGS = -L$(LIBFT_DIR) -lft
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LINK_FLAGS) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	$(RM) $(OBJS)

fclean:
	@$(MAKE) -C $(LIBFT_DIR) fclean
	$(RM) $(OBJS)
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
