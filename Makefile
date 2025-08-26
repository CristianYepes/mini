# =========================
# Makefile - Tokenizer Demo + Tests
# =========================

NAME        := lexer_demo
TEST_NAME   := lexer_tests

CC          := cc
CFLAGS      := -Wall -Wextra -Werror -MMD -MP
INCS        := -Iincludes -Ilibft

LIBFT_DIR   := libft
LIBFT_A     := $(LIBFT_DIR)/libft.a

# Linux: suele bastar con -lreadline; si falla:
# make RL_LIBS="-lreadline -lncurses"
RL_LIBS     ?= -lreadline

# ------- Fuentes comunes (lexer) -------
COMMON_SRCS := \
  src/lex/lex_buf.c \
  src/lex/lex_nodes.c \
  src/lex/lex_emit.c \
  src/lex/lex_scan.c \
  src/lex/lex_helpers.c \
  src/lex/unquote_pass.c \
  src/lex/post_retokenize.c \
  src/lex/token_free.c

# Demo interactiva (usa readline)
SRCS        := $(COMMON_SRCS) main.c
OBJS        := $(SRCS:.c=.o)
DEPS        := $(OBJS:.o=.d)

# Binario de tests (no usa readline)
TEST_SRCS   := $(COMMON_SRCS) test/lexer_tests.c
TEST_OBJS   := $(TEST_SRCS:.c=.o)
TEST_DEPS   := $(TEST_OBJS:.o=.d)

# ---------- Rules ----------
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(INCS) $(OBJS) $(LIBFT_A) $(RL_LIBS) -o $@

$(TEST_NAME): $(LIBFT_A) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCS) $(TEST_OBJS) $(LIBFT_A) -o $@

# Construye libft con bonus (para ft_lstclear & co)
$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(MAKE) -C $(LIBFT_DIR) bonus

# Regla genérica de compilación
%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

# Atajos
run: $(NAME)
	./$(NAME)

test: $(TEST_NAME)
	./$(TEST_NAME)

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(DEPS) $(TEST_OBJS) $(TEST_DEPS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(TEST_NAME)

re: fclean all

-include $(DEPS) $(TEST_DEPS)

.PHONY: all clean fclean re test run
