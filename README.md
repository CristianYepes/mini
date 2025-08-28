README — Tokenizer (Lexer) de Minishell
1) Idea general

El lexer convierte una línea de entrada en una lista enlazada de tokens (t_list de tu libft). Los tipos admitidos:

ARG → palabras normales (incluye flags como -la, rutas, etc.)

PIPE → |

FILE_REDIR → <, >, <<, >>

LOGIC → &&, || (bonus si lo usas)

Reglas clave:

Los operadores se reconocen fuera de comillas.

Las comillas (' ") agrupan texto: el lexer copia su contenido y permite adyacencia ("a"b ⇒ ab).

Dentro de " aceptamos solo la secuencia \" para producir un " literal (sirve para tu test 72).

Cada token lleva un flag quoted_by (0, ', ").

Luego, ft_remove_quotes solo elimina el par exterior si el token nació entrecomillado por ese tipo (Opción B).

Comillas literales (p.ej. ' dentro de "...") no se tocan.

El flujo:

entrada
  → tokenizer()
      - salta espacios
      - operador? → token FILE_REDIR / PIPE / LOGIC
      - si no, palabra ARG (respetando comillas)
  → ft_remove_quotes()  (quita SOLO par exterior si coincide quoted_by)
  → (retokenize si te hace falta para fases posteriores)

2) Compilar y ejecutar
make          # build demo interactiva (usa readline)
./lexer_demo

# targets útiles
make clean
make fclean
make re

3) Memoria y errores

Todos los tokens se limpian con ft_lstclear(&lst, free_cmd_tok).

Ante fallo en mitad del lexing, liberamos lo ya construido y devolvemos NULL.

buf_* evita realocar a cada char y simplifica gestión.

4) Ficheros y funciones
includes/structs.h

t_token { char *str; int type; char quoted_by; }
quoted_by: 0 si no nació entre comillas; ' o " si nació entrecomillado completamente.

t_buf { char *data; size_t len; size_t cap; }
buffer dinámico de apoyo.

t_scan { t_buf *acc; int *err; }
mini-paquete para no pasar >4 args en helpers de lectura.

includes/minishell.h

Manifiesto de la API pública del lexer:

Buffer: buf_init, buf_pushc, buf_steal.

Helpers: ft_isreserved, skip_spaces_alt, read_operator_alt, lex_handle_op.

Emisores/Nodos: push_token, free_cmd_tok, emit_empty_arg, emit_arg_word, emit_operator_if_any, tok_set_quoted_by.

Fases: tokenizer, ft_remove_quotes, retokenize.

Utils retokenize: vec_grow, replace_token_head.

src/lex/lex_buf.c

Objetivo: utilidades de buffer dinámico (pequeñas y Norm-friendly).

void buf_init(t_buf *b)
Inicializa {data=NULL, len=0, cap=0}.

int buf_pushc(t_buf *b, char c)
Asegura capacidad (doblando) y añade c. Mantiene data[len]='\0'.
Devuelve 0/1 (OK/ERR).

char *buf_steal(t_buf *b)
Transfiere el data al llamador y deja el t_buf en {NULL,0,0}.

Notas: No usa realloc por norma; implementa “crecer y copiar”.

src/lex/lex_nodes.c

Objetivo: crear/editar nodos de la lista y del token.

int push_token(t_list **tail, char *s, int type)
Reserva t_token y t_list, rellena, y cuelga tras *tail.
No duplica s: le pasas memoria propia (normalmente desde buf_steal).

void tok_set_quoted_by(t_list *tail, char q)
Marca quoted_by del último token creado.

int replace_token_head(t_token *orig, char *new_str, int type)
Reutiliza el t_token de cabecera cambiando str y type.

src/lex/lex_helpers.c

Objetivo: pequeñas utilidades de lectura.

char ft_isreserved(char c)
1 si |, &, <, >; si añades ; como bonus, aquí también.

int skip_spaces_alt(char *s, int i, int *start, int set_start)
Devuelve el nuevo índice moviéndose sobre espacios (isspace).
Si set_start, también escribe *start = nuevo_i.

int read_operator_alt(const char *s, int i, char out[3], int *type_out)
Si hay operador en s[i], escribe su texto a out (máx 2 chars),
fija *type_out y devuelve el índice tras el operador.
Si no lo hay, *type_out=0 y devuelve i.

int lex_handle_op(const char *s, int i, t_list **tail)
Usa read_operator_alt y, si hay operador, emite el token y devuelve
el nuevo índice; si no, devuelve i sin cambios. En error: -1.

src/lex/lex_emit.c

Objetivo: funciones “de emisión” usadas por el escáner.

int emit_operator_if_any(const char *s, int i, t_list **tail, int *did)
Igual que lex_handle_op pero te dice con *did si emitió.

int emit_empty_arg(t_list **tail)
Emite un ARG vacío ("" o '').

int emit_arg_word(const char *s, int i, t_list **tail)
Llama a read_word_alt, crea el token con push_token,
y marca quoted_by si la palabra estaba completa entre comillas (ver lex_scan.c).

src/lex/lex_scan.c

Objetivo: escaneo “palabra a palabra”.

static int read_quoted_alt(const char *s, int i, char q, t_scan sc)
Copia el contenido de '...' o "...".
Dentro de ": acepta solo \" para producir " literal (no más escapes).
Si no encuentra cierre → copia la comilla de apertura y trata como texto normal.

static int read_word_alt(const char *s, int i, t_buf *acc, int *err)
Construye una palabra hasta espacio u operador sin entrar en comillas,
pero cuando ve ' o " delega en read_quoted_alt para copiar el interior.
Devuelve el nuevo índice y deja la palabra en acc.

int emit_arg_word(const char *s, int i, t_list **tail)
Orquesta lo anterior: si acc.len == 0 pero hubo algo (""), emite ARG vacío.
Si hay palabra, push_token con buf_steal.
Marcado quoted_by: si s[start] era ' / " y cierra en s[i-1],
entonces tok_set_quoted_by(*tail, s[start]).

static int step_lex(const char *str, int i, t_list **tail)
Un paso del bucle: salta espacios, intenta operador, si no, emite palabra.

t_list *tokenizer(char *str)
Bucle principal: repite step_lex hasta fin o error.
Devuelve la lista de tokens.

src/lex/unquote_pass.c (Opción B con quoted_by)

Objetivo: quitar solo el par exterior de comillas si el token nació entrecomillado por ese tipo.

static int is_wrapped(const char *s, char q)
Comprueba que s sea q ... q y no haya nada más.

static void strip_outer(char *s)
Elimina ese par exterior in-place (desplaza memoria).

int ft_remove_quotes(t_list *tokens)
Recorre la lista; si tk->quoted_by == q y is_wrapped(tk->str,q),
aplica strip_outer. Comillas internas se conservan.

src/lex/post_retokenize.c

Objetivo: trocear el str de un token en varios tokens a partir de un offset (útil para fases posteriores como expansión/globbing).

Funciones típicas dentro (puede variar con tu versión):

static int scan_word(const char *in, int i, t_buf *acc)
Lee una palabra fuera de comillas con comillas balanceadas (similar a read_word_alt).

int vec_grow(char ***vec, int *cap, int pieces)
Crece el vector de char* donde guardas piezas.

static int split_outside_quotes(const char *in, char ***out_vec)
Divide in en piezas por espacios respetando comillas.

int replace_token_head(t_token *orig, char *new_str, int type)
(Implementado en lex_nodes.c) Reutiliza el primer token.

int retokenize(t_list *curr, int type, int start, int *lngths)
Corta curr->content->str por start, divide en piezas, sustituye el head
con la primera pieza y encadena nuevas piezas como tokens del mismo tipo.

src/lex/token_free.c

void free_cmd_tok(void *tok)
free(t->str); free(t); — callback para ft_lstclear.

main.c

Demo muy simple: lee una línea con readline(), tokeniza, llama a ft_remove_quotes, imprime tokens y libera.

5) Decisiones importantes (y por qué)

quoted_by + unquote “outer-only”: Evita romper comillas literales y los casos de “comillas anidadas” (" 'a' ", ' "a" ', etc.).

\" solo en ": suficiente para permitir " dentro de comillas dobles sin activar un sistema completo de escapes (que el subject no pide).

Concatenación de segmentos entrecomillados: "a""b" ⇒ ab (igual que bash). Por eso " > "" >> " ⇒ >>>.

6) Cómo extender (bonus)

Añadir ; al set de operadores: ampliar ft_isreserved() y op_type.

Añadir más escapes dentro de ": ampliar read_quoted_alt con casos concretos.

Wildcards *: después del lexer, en una fase de expansión, sobre tokens ARG.

“Fichas” para pegar en cada .c (opcional)

Pega al principio de cada archivo (tras el header 42) una versión resumida de su objetivo + lista de funciones; por ejemplo, en lex_scan.c:

/*
** lex_scan.c — escaneo de palabras y bucle del tokenizer
**
** read_quoted_alt : copia contenido de '...' o "..." (en "..." acepta \" -> ")
** read_word_alt   : construye una palabra fuera de comillas respetándolas
** emit_arg_word   : emite ARG (vacío si la palabra fue ""/''), marca quoted_by
** step_lex        : salta espacios, intenta operador; si no, palabra
** tokenizer       : bucle principal; devuelve lista de tokens o NULL en error
*/
