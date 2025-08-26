#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "structs.h"

/* Buffer compartido */
void	buf_init(t_buf *b);
int		buf_pushc(t_buf *b, char c);
char	*buf_steal(t_buf *b);
int		vec_grow(char ***vec, int *cap, int pieces);

/* Helpers del lexer */
char	ft_isreserved(char c);
int		skip_spaces_alt(char *s, int i, int *start, int set_start);
int		read_operator_alt(const char *s, int i, char out[3], int *type_out);

/* Emitters compartidos */
int		emit_operator_if_any(const char *s, int i, t_list **tail, int *did);
int		emit_arg_word(const char *s, int i, t_list **tail);
int		emit_empty_arg(t_list **tail);

/* Auxiliares */
int		lex_handle_op(const char *s, int i, t_list **tail);
int		replace_token_head(t_token *orig, char *new_str, int type);

/* Fases */
t_list	*tokenizer(char *str);
int		ft_remove_quotes(t_list *tokens);
int		retokenize(t_list *curr, int type, int start, int *lngths);
int		state_quote_delimiter_alt(char *str, int i, char delim);

/* Util */
int		push_token(t_list **tail, char *s, int type);
void	free_cmd_tok(void *tok);

#endif
