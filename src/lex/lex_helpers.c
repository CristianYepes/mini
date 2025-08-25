#include "minishell.h"

/* Parte obligatoria: ; NO es reservado. Para bonus, mira el comentario en op_type. */
char	ft_isreserved(char c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>')
		return (1);
	return (0);
}

/* ALT: devuelve nuevo índice y opcionalmente fija *start */
int	skip_spaces_alt(char *s, int i, int *start, int set_start)
{
	if (!s)
		return (i);
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;
	if (set_start && start)
		*start = i;
	return (i);
}

static int	op_type(const char *op)
{
	if (op[0] == '|' && op[1] == '\0')
		return (PIPE);
	if ((op[0] == '&' && op[1] == '&') || (op[0] == '|' && op[1] == '|'))
		return (LOGIC);
	if (op[0] == '<' || op[0] == '>')
		return (FILE_REDIR);
	/* BONUS: si decides soportar ';', añade:
	** if (op[0] == ';' && op[1] == '\0') return (LOGIC);
	*/
	return (ARG);
}

/* Lee operador y devuelve NUEVO i; setea *type_out (0 si no hay op) */
int	read_operator_alt(const char *s, int i, char out[3], int *type_out)
{
	int	type;

	out[0] = '\0';
	out[1] = '\0';
	out[2] = '\0';
	type = 0;
	if (!s || !s[i] || !ft_isreserved(s[i]))
	{
		*type_out = ARG;
		return (i);
	}
	out[0] = s[i];
	i++;
	if (s[i] && s[i] == out[0]
		&& (out[0] == '>' || out[0] == '<' || out[0] == '|' || out[0] == '&'))
	{
		out[1] = s[i];
		i++;
	}
	type = op_type(out);
	*type_out = type;
	return (i);
}
