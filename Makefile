# =========================
# Makefile - Tokenizer Demo
# =========================

NAME        := lexer_demo

CC          := cc
CFLAGS      := -Wall -Wextra -Werror -MMD -MP
INCS        := -Iincludes -Ilibft

LIBFT_DIR   := libft
LIBFT_A     := $(LIBFT_DIR)/libft.a

# --- Readline (ajusta si hace falta) ---
# Linux suele valer con:
RL_LIBS     ?= -lreadline
# En algunas distros:
# RL_LIBS   ?= -lreadline -lncurses
# macOS (Homebrew):
# RL_CFLAGS ?= -I/opt/homebrew/opt/readline/include
# RL_LDFLAGS?= -L/opt/homebrew/opt/readline/lib

# --- Fuentes ---
LEX_SRCS := \
	src/lex/lex_helpers.c \
	src/lex/lex_scan.c \
	src/lex/post_retokenize.c \
	src/lex/token_free.c
	src/lex/unquote_pass.c \

MAIN_SRCS := main.c

SRCS := $(LEX_SRCS) $(MAIN_SRCS)
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)

# ---------- Rules ----------
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) $(INCS) $(RL_CFLAGS) $(OBJS) $(LIBFT_A) $(RL_LDFLAGS) $(RL_LIBS) -o $@

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) $(RL_CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(DEPS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
