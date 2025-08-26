#include "minishell.h"

/* ===== helpers ===== */
static const char	*type_name(int t)
{
	if (t == FILE_REDIR) return ("FILE_REDIR");
	if (t == PIPE) return ("PIPE");
	if (t == LOGIC) return ("LOGIC");
	if (t == ARG) return ("ARG");
	return ("UNKNOWN");
}

static int	buf_pushs(t_buf *b, const char *s)
{
	size_t i = 0;
	while (s && s[i])
	{
		if (buf_pushc(b, s[i]))
			return (1);
		i++;
	}
	return (0);
}

static char	*dump_tokens(t_list *lst)
{
	t_buf out;

	buf_init(&out);
	while (lst)
	{
		t_token *tk = (t_token *)lst->content;
		if (tk && tk->str)
		{
			if (buf_pushc(&out, '[')) return (free(out.data), NULL);
			if (buf_pushs(&out, tk->str)) return (free(out.data), NULL);
			if (buf_pushs(&out, "] (")) return (free(out.data), NULL);
			if (buf_pushs(&out, type_name(tk->type))) return (free(out.data), NULL);
			if (buf_pushs(&out, ")\n")) return (free(out.data), NULL);
		}
		lst = lst->next;
	}
	return buf_steal(&out);
}

/* ===== casos ===== */
typedef struct s_case { const char *input; const char *expect; } t_case;

static const t_case CASES[] = {
/* 1-4 ARG básicos */
{ "ls -la",
  "[ls] (ARG)\n[-la] (ARG)\n" },
{ "echo hola mundo",
  "[echo] (ARG)\n[hola] (ARG)\n[mundo] (ARG)\n" },
{ "echo \"hola mundo\"",
  "[echo] (ARG)\n[hola mundo] (ARG)\n" },
{ "echo \"\" \"\"",
  "[echo] (ARG)\n[] (ARG)\n[] (ARG)\n" },
/* 5-7 PIPE */
{ "echo a|wc -c",
  "[echo] (ARG)\n[a] (ARG)\n[|] (PIPE)\n[wc] (ARG)\n[-c] (ARG)\n" },
{ "echo a | wc",
  "[echo] (ARG)\n[a] (ARG)\n[|] (PIPE)\n[wc] (ARG)\n" },
{ "echo \"|\" x",
  "[echo] (ARG)\n[|] (ARG)\n[x] (ARG)\n" },
/* 8-22 FILE_REDIR */
{ "echo hi > out.txt",
  "[echo] (ARG)\n[hi] (ARG)\n[>] (FILE_REDIR)\n[out.txt] (ARG)\n" },
{ "echo hi>out.txt",
  "[echo] (ARG)\n[hi] (ARG)\n[>] (FILE_REDIR)\n[out.txt] (ARG)\n" },
{ "cat < in.txt",
  "[cat] (ARG)\n[<] (FILE_REDIR)\n[in.txt] (ARG)\n" },
{ "cat<in.txt",
  "[cat] (ARG)\n[<] (FILE_REDIR)\n[in.txt] (ARG)\n" },
{ "cmd<in >out",
  "[cmd] (ARG)\n[<] (FILE_REDIR)\n[in] (ARG)\n[>] (FILE_REDIR)\n[out] (ARG)\n" },
{ "echo hi >> out.txt",
  "[echo] (ARG)\n[hi] (ARG)\n[>>] (FILE_REDIR)\n[out.txt] (ARG)\n" },
{ "echo hi>>out.txt",
  "[echo] (ARG)\n[hi] (ARG)\n[>>] (FILE_REDIR)\n[out.txt] (ARG)\n" },
{ "cat << EOF",
  "[cat] (ARG)\n[<<] (FILE_REDIR)\n[EOF] (ARG)\n" },
{ "cat<<EOF",
  "[cat] (ARG)\n[<<] (FILE_REDIR)\n[EOF] (ARG)\n" },
{ "echo \"a>b\" x",
  "[echo] (ARG)\n[a>b] (ARG)\n[x] (ARG)\n" },
{ "echo 'a<b' y",
  "[echo] (ARG)\n[a<b] (ARG)\n[y] (ARG)\n" },
{ "cmd >>> file",
  "[cmd] (ARG)\n[>>] (FILE_REDIR)\n[>] (FILE_REDIR)\n[file] (ARG)\n" },
{ "cmd<< <file",
  "[cmd] (ARG)\n[<<] (FILE_REDIR)\n[<] (FILE_REDIR)\n[file] (ARG)\n" },
{ "> out.txt",
  "[>] (FILE_REDIR)\n[out.txt] (ARG)\n" },
{ "< in.txt cmd",
  "[<] (FILE_REDIR)\n[in.txt] (ARG)\n[cmd] (ARG)\n" },
/* 23-25 LOGIC */
{ "true && echo ok",
  "[true] (ARG)\n[&&] (LOGIC)\n[echo] (ARG)\n[ok] (ARG)\n" },
{ "false||echo alt",
  "[false] (ARG)\n[||] (LOGIC)\n[echo] (ARG)\n[alt] (ARG)\n" },
{ "echo \"a && b\" || echo c",
  "[echo] (ARG)\n[a && b] (ARG)\n[||] (LOGIC)\n[echo] (ARG)\n[c] (ARG)\n" },
/* 26-30 combinaciones */
{ "cmd <<EOF|wc > result",
  "[cmd] (ARG)\n[<<] (FILE_REDIR)\n[EOF] (ARG)\n[|] (PIPE)\n[wc] (ARG)\n[>] (FILE_REDIR)\n[result] (ARG)\n" },
{ "grep \">\" file | cat <<EOF",
  "[grep] (ARG)\n[>] (ARG)\n[file] (ARG)\n[|] (PIPE)\n[cat] (ARG)\n[<<] (FILE_REDIR)\n[EOF] (ARG)\n" },
{ "echo \"<\" \">\" '<<' \">>\"",
  "[echo] (ARG)\n[<] (ARG)\n[>] (ARG)\n[<<] (ARG)\n[>>] (ARG)\n" },
{ "echo a|b|c||d&&e>f<g",
  "[echo] (ARG)\n[a] (ARG)\n[|] (PIPE)\n[b] (ARG)\n[|] (PIPE)\n[c] (ARG)\n[||] (LOGIC)\n[d] (ARG)\n[&&] (LOGIC)\n[e] (ARG)\n[>] (FILE_REDIR)\n[f] (ARG)\n[<] (FILE_REDIR)\n[g] (ARG)\n" },
{ "echo \">\"a",
  "[echo] (ARG)\n[>a] (ARG)\n" },
/* 31-35 más ARG y comillas */
{ "echo \"\"",
  "[echo] (ARG)\n[] (ARG)\n" },
{ "\"\"",
  "[] (ARG)\n" },
{ "echo \" \" \"  \"",
  "[echo] (ARG)\n[ ] (ARG)\n[  ] (ARG)\n" },
{ "echo 'a b' \"c d\"",
  "[echo] (ARG)\n[a b] (ARG)\n[c d] (ARG)\n" },
{ "echo '|' \"<\" \">\"",
  "[echo] (ARG)\n[|] (ARG)\n[<] (ARG)\n[>] (ARG)\n" },
/* 36-40 heredocs y combos */
{ "cmd<<EOF|tr a b<<XX",
  "[cmd] (ARG)\n[<<] (FILE_REDIR)\n[EOF] (ARG)\n[|] (PIPE)\n[tr] (ARG)\n[a] (ARG)\n[b] (ARG)\n[<<] (FILE_REDIR)\n[XX] (ARG)\n" },
{ "cmd<in>>out",
  "[cmd] (ARG)\n[<] (FILE_REDIR)\n[in] (ARG)\n[>>] (FILE_REDIR)\n[out] (ARG)\n" },
{ "cmd<>file",
  "[cmd] (ARG)\n[<] (FILE_REDIR)\n[>] (FILE_REDIR)\n[file] (ARG)\n" },
{ "<<EOF",
  "[<<] (FILE_REDIR)\n[EOF] (ARG)\n" },
{ "echo a&&b",
  "[echo] (ARG)\n[a] (ARG)\n[&&] (LOGIC)\n[b] (ARG)\n" },
/* 41-45 lógicas y redirs */
{ "echo a ||b",
  "[echo] (ARG)\n[a] (ARG)\n[||] (LOGIC)\n[b] (ARG)\n" },
{ "a|b||c&&d|e",
  "[a] (ARG)\n[|] (PIPE)\n[b] (ARG)\n[||] (LOGIC)\n[c] (ARG)\n[&&] (LOGIC)\n[d] (ARG)\n[|] (PIPE)\n[e] (ARG)\n" },
{ "echo 'hi'\"\"'x'",
  "[echo] (ARG)\n[hix] (ARG)\n" },
{ "echo \"\">a",
  "[echo] (ARG)\n[] (ARG)\n[>] (FILE_REDIR)\n[a] (ARG)\n" },
{ ">out <in",
  "[>] (FILE_REDIR)\n[out] (ARG)\n[<] (FILE_REDIR)\n[in] (ARG)\n" },
/* 46-50 espacios, tabs y adyacencias */
{ "< in | cat",
  "[<] (FILE_REDIR)\n[in] (ARG)\n[|] (PIPE)\n[cat] (ARG)\n" },
{ "echo\targ", /* tab */
  "[echo] (ARG)\n[arg] (ARG)\n" },
{ "   echo   a   ",
  "[echo] (ARG)\n[a] (ARG)\n" },
{ "\"\" \"\" \"\"",
  "[] (ARG)\n[] (ARG)\n[] (ARG)\n" },
{ "\"a\"\"b\"\"c\"",
  "[abc] (ARG)\n" },
/* 51-55 adyacencias y comillas mixtas */
{ "cmd \"a\"b",
  "[cmd] (ARG)\n[ab] (ARG)\n" },
{ "echo 'ab\"cd'",
  "[echo] (ARG)\n[ab\"cd] (ARG)\n" },
{ "echo \"ab'cd\"",
  "[echo] (ARG)\n[ab'cd] (ARG)\n" },
{ "echo \"a\\b\"",
  "[echo] (ARG)\n[a\\b] (ARG)\n" },
{ "false && true || echo ok",
  "[false] (ARG)\n[&&] (LOGIC)\n[true] (ARG)\n[||] (LOGIC)\n[echo] (ARG)\n[ok] (ARG)\n" },
/* 56-60 variantes finales */
{ "cmd>>>file",
  "[cmd] (ARG)\n[>>] (FILE_REDIR)\n[>] (FILE_REDIR)\n[file] (ARG)\n" },
{ "cmd <<  EOF",
  "[cmd] (ARG)\n[<<] (FILE_REDIR)\n[EOF] (ARG)\n" },
{ "echo \"a  b\"",
  "[echo] (ARG)\n[a  b] (ARG)\n" },
{ "echo ' '",
  "[echo] (ARG)\n[ ] (ARG)\n" },
{ "echo",
  "[echo] (ARG)\n" },
};

static int	run_one(int idx, const char *input, const char *expect)
{
	t_list	*toks;
	char	*out;
	int		ok;

	toks = tokenizer((char *)input);
	if (!toks)
	{
		printf("CASE %d: tokenizer returned NULL\n", idx + 1);
		return (0);
	}
	ft_remove_quotes(toks);
	out = dump_tokens(toks);
	ok = (out && expect && ft_strncmp(out, expect, ft_strlen(expect) + 1) == 0);
	if (!ok)
	{
		printf("CASE %d FAILED\nINPUT : %s\nEXPECT:\n%sGOT:\n%s\n",
			idx + 1, input, expect ? expect : "(null)\n",
			out ? out : "(null)\n");
	}
	free(out);
	ft_lstclear(&toks, free_cmd_tok);
	return (ok);
}

int	main(void)
{
	int i, pass = 0, total;

	total = (int)(sizeof(CASES) / sizeof(CASES[0]));
	for (i = 0; i < total; ++i)
		pass += run_one(i, CASES[i].input, CASES[i].expect);
	printf("\nSummary: %d / %d passed\n", pass, total);
	return (pass == total ? 0 : 1);
}
