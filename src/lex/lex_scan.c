/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_scan.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 22:09:24 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/26 18:04:53 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_quoted_alt(const char *s, int i, char q, t_scan sc)
{
	i++;
	while (s[i] && s[i] != q)
	{
		if (buf_pushc(sc.acc, s[i]))
		{
			*(sc.err) = 1;
			return (i);
		}
		i++;
	}
	if (s[i] == q)
		i++;
	return (i);
}

static int	read_word_alt(const char *s, int i, t_buf *acc, int *err)
{
	t_scan	sc;

	sc.acc = acc;
	sc.err = err;
	while (s[i])
	{
		if (ft_isreserved(s[i]) || ft_isspace((int)(unsigned char)s[i]))
			break ;
		if (s[i] == '\'' || s[i] == '\"')
		{
			i = read_quoted_alt(s, i, s[i], sc);
			if (*err)
				return (i);
			continue ;
		}
		if (buf_pushc(acc, s[i]))
		{
			*err = 1;
			return (i);
		}
		i++;
	}
	return (i);
}

int	emit_arg_word(const char *s, int i, t_list **tail)
{
	t_buf	acc;
	int		err;
	int		start;

	buf_init(&acc);
	err = 0;
	start = i;
	i = read_word_alt(s, i, &acc, &err);
	if (err)
		return (free(acc.data), -1);
	if (acc.len == 0)
	{
		free(acc.data);
		/* si hubo algo (p. ej., "" o ''), emitir ARG vacío */
		if (i != start)
		{
			if (emit_empty_arg(tail))
				return (-1);
		}
		return (i);
	}
	if (push_token(tail, buf_steal(&acc), ARG))
		return (-1);
	return (i);
}

/* Un paso de lexing: salta espacios, operador o palabra.
   Devuelve: nuevo índice, -1 error, -2 fin de cadena. */
static int	step_lex(const char *str, int i, t_list **tail)
{
	int	nxt;

	i = skip_spaces_alt((char *)str, i, NULL, 0);
	if (!str[i])
		return (-2);
	nxt = lex_handle_op(str, i, tail);
	if (nxt < 0)
		return (-1);
	if (nxt != i)
		return (nxt);
	return (emit_arg_word(str, i, tail));
}

t_list	*tokenizer(char *str)
{
	t_list	head;
	t_list	*tail;
	int		i;
	int		r;

	head.next = NULL;
	tail = &head;
	i = 0;
	while (str && str[i])
	{
		r = step_lex(str, i, &tail);
		if (r == -1)
			return (ft_lstclear(&head.next, free_cmd_tok), NULL);
		if (r == -2)
			break ;
		i = r;
	}
	return (head.next);
}
