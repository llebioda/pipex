CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCFILES = pipex.c pipex_utils.c ft_calloc.c ft_str.c ft_split.c
SRCDIR = ./
SRCS = $(addprefix $(SRCDIR), $(SRCFILES))
OBJDIR = ./build/
OBJS = $(addprefix $(OBJDIR), $(SRCFILES:.c=.o))
DEPS = $(OBJS:.o=.d)

HEADERFILES = pipex.h
HEADERDIR = ./
HEADERS = $(addprefix $(HEADERDIR), $(HEADERFILES))
INCLUDES = -I $(HEADERDIR)

NAME = pipex

BONUS_SRCFILES = pipex_bonus.c pipex_utils.c ft_calloc.c ft_str.c ft_split.c
BONUS_OBJS = $(addprefix $(OBJDIR), $(BONUS_SRCFILES:.c=.o))
BONUS_DEPS = $(OBJS:.o=.d)
BONUS_NAME = pipex_bonus

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJS)
	$(CC) $(CFLAGS) -o $(BONUS_NAME) $(BONUS_OBJS)

$(OBJDIR)%.o: $(SRCDIR)%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MF $(@:.o=.d) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJS) $(DEPS) $(BONUS_OBJS) $(BONUS_DEPS)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean
	@$(MAKE) --no-print-directory all

-include $(DEPS) $(BONUS_DEPS)

.PHONY: all clean fclean re bonus
