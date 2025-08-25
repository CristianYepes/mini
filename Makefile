# =========================
# Makefile - Tokenizer Demo
# =========================

NAME        := lexer_demo

CC          := cc
CFLAGS      := -Wall -Wextra -Werror -MMD -MP
INCS        := -Iincludes -Ilibft

LIBFT_DIR   := libft
LIBFT_A     := $(LIBFT_DIR)/libft.a

# Linux: suele bastar con -lreadline; si falla, usa: make RL_LIBS="-lreadline -lncurses"
RL_LIBS     ?= -lreadline

SRCS := \
  src/lex/lex_scan.c \
  src/lex/lex_helpers.c \
  src/lex/unquote_pass.c \
  src/lex/post_retokenize.c \
  src/lex/token_free.c \
  main.c

OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

# ---------- Rules ----------
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(INCS) $(OBJS) $(LIBFT_A) $(RL_LIBS) -o $@

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(LIBFT_DIR) bonus

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(DEPS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
