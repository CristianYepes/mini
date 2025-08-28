# README — Tokenizer (Lexer) de Minishell

## 1) Idea general

El lexer convierte una línea de entrada en una **lista enlazada de tokens** (`t_list` de tu libft).

### Tipos admitidos
- **ARG** → palabras normales (incluye flags como `-la`, rutas, etc.)
- **PIPE** → `|`
- **FILE_REDIR** → `<`, `>`, `<<`, `>>`
- **LOGIC** → `&&`, `||` (bonus si lo usas)

### Reglas clave
- Los operadores se reconocen **fuera de comillas**.
- Las comillas (`'`, `"`) agrupan texto: el lexer copia su contenido y permite adyacencia (`"a"b` ⇒ `ab`).
- Dentro de `"..."` solo se acepta la secuencia `\"` para producir un `"` literal.
- Cada token lleva un flag **`quoted_by`** (`0`, `'`, `"`).
- `ft_remove_quotes` elimina únicamente el par exterior si coincide con `quoted_by` (**opción B**).
- Las comillas literales internas (ej. `'` dentro de `"..."`) se conservan.

### Flujo
```txt
entrada
  → tokenizer()
      - salta espacios
      - operador? → token FILE_REDIR / PIPE / LOGIC
      - si no, palabra ARG (respetando comillas)
  → ft_remove_quotes()   (quita SOLO par exterior si coincide quoted_by)
  → (retokenize si hace falta para fases posteriores)
```

---

## 2) Compilar y ejecutar

```sh
make          # build demo interactiva (usa readline)
./lexer_demo
```

### Targets útiles
```sh
make clean
make fclean
make re
```

---

## 3) Memoria y errores
- Todos los tokens se limpian con `ft_lstclear(&lst, free_cmd_tok)`.
- Ante fallo en mitad del lexing → se libera lo ya construido y se devuelve `NULL`.
- Los helpers `buf_*` evitan realocar carácter a carácter y simplifican la gestión.

---

## 4) Ficheros y funciones

### `includes/structs.h`
```c
t_token { char *str; int type; char quoted_by; }
```
- `quoted_by`: `0` si no nació entre comillas; `'` o `"` si sí.

```c
t_buf { char *data; size_t len; size_t cap; }
```
- Buffer dinámico auxiliar.

```c
t_scan { t_buf *acc; int *err; }
```
- Mini-paquete para no pasar más de 4 args en helpers.

---

### `includes/minishell.h`
Manifiesto de la API pública del lexer:

- **Buffer:** `buf_init`, `buf_pushc`, `buf_steal`
- **Helpers:** `ft_isreserved`, `skip_spaces_alt`, `read_operator_alt`, `lex_handle_op`
- **Emisores/Nodos:** `push_token`, `free_cmd_tok`, `emit_empty_arg`, `emit_arg_word`, `emit_operator_if_any`, `tok_set_quoted_by`
- **Fases:** `tokenizer`, `ft_remove_quotes`, `retokenize`
- **Utils retokenize:** `vec_grow`, `replace_token_head`

---

### `src/lex/lex_buf.c`
**Objetivo:** utilidades de buffer dinámico (pequeñas y Norm-friendly).

- `void buf_init(t_buf *b)` → inicializa `{data=NULL, len=0, cap=0}`
- `int buf_pushc(t_buf *b, char c)` → asegura capacidad (doblando) y añade `c`
- `char *buf_steal(t_buf *b)` → transfiere `data` al llamador y reinicia el `t_buf`

> No usa `realloc`; implementa “crecer y copiar”.

---

### `src/lex/lex_nodes.c`
**Objetivo:** crear/editar nodos de la lista y del token.

- `int push_token(t_list **tail, char *s, int type)`
- `void tok_set_quoted_by(t_list *tail, char q)`
- `int replace_token_head(t_token *orig, char *new_str, int type)`

---

### `src/lex/lex_helpers.c`
**Objetivo:** utilidades de lectura.

- `char ft_isreserved(char c)` → detecta `|`, `&`, `<`, `>` (añade `;` como bonus)
- `int skip_spaces_alt(...)` → avanza sobre espacios
- `int read_operator_alt(...)` → detecta operadores de hasta 2 chars
- `int lex_handle_op(...)` → emite operador si lo hay

---

### `src/lex/lex_emit.c`
**Objetivo:** funciones de emisión.

- `int emit_operator_if_any(...)`
- `int emit_empty_arg(...)` → emite un ARG vacío (`""` o `''`)
- `int emit_arg_word(...)`

---

### `src/lex/lex_scan.c`
**Objetivo:** escaneo palabra a palabra.

- `read_quoted_alt(...)` → copia `'...'` o `"..."`, admite `\"` en `"..."`
- `read_word_alt(...)` → construye palabra, delega en `read_quoted_alt` si encuentra comillas
- `emit_arg_word(...)` → crea token ARG (vacío si `""`) y marca `quoted_by`
- `step_lex(...)` → salta espacios, prueba operador, si no palabra
- `tokenizer(...)` → bucle principal del lexer

---

### `src/lex/unquote_pass.c` (opción B con `quoted_by`)
**Objetivo:** quitar solo el **par exterior** de comillas si el token nació entrecomillado por ese tipo.

- `is_wrapped(...)` → comprueba que `s` esté entre `q...q`
- `strip_outer(...)` → elimina comillas exteriores *in-place*
- `ft_remove_quotes(...)` → aplica la lógica sobre toda la lista

---

### `src/lex/post_retokenize.c`
**Objetivo:** dividir un token en varios tokens (útil para expansión/globbing).

- `scan_word(...)` → lee palabra con comillas balanceadas
- `vec_grow(...)` → expande vector de piezas
- `split_outside_quotes(...)` → divide string respetando comillas
- `replace_token_head(...)`
- `retokenize(...)`

---

### `src/lex/token_free.c`
- `void free_cmd_tok(void *tok)` → `free(t->str); free(t);`

---

### `main.c`
Demo muy simple:
1. Lee una línea con `readline()`
2. Tokeniza
3. Llama a `ft_remove_quotes`
4. Imprime tokens
5. Libera memoria

---

## 5) Decisiones importantes
- `quoted_by` + unquote “outer-only” → evita romper comillas literales y casos de anidamiento.
- `\"` solo en `"` → suficiente para meter `"` literal sin un sistema completo de escapes.
- Concatenación de segmentos entrecomillados: `"a""b"` ⇒ `ab`.
  Ejemplo: `" > "" >> "` ⇒ `>>>`.

---

## 6) Cómo extender (bonus)
- Añadir `;` al set de operadores.
- Añadir más escapes en `read_quoted_alt`.
- Implementar wildcards `*` en la fase de expansión tras el lexer.

---

## 7) Fichas para cada `.c` (opcional)

Ejemplo para `lex_scan.c`:

```c
/*
** lex_scan.c — escaneo de palabras y bucle del tokenizer
**
** read_quoted_alt : copia contenido de '...' o "..." (en "..." acepta \" -> ")
** read_word_alt   : construye palabra fuera de comillas respetándolas
** emit_arg_word   : emite ARG (vacío si fue ""/''), marca quoted_by
** step_lex        : salta espacios, intenta operador; si no, palabra
** tokenizer       : bucle principal; devuelve lista de tokens o NULL en error
*/
```
