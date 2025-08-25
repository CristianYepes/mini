/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_scan.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcarpio-cyepes <rcarpio-cyepes@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 22:09:24 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/25 20:26:21 by rcarpio-cye      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* prototipo del helper ALT */
int	read_operator_alt(const char *s, int i, char out[3], int *type_out);

typedef struct s_buf
{
	char	*data;
	size_t	len;
	size_t	cap;
}	t_buf;

/* --- buffer simple --- */
static int	buf_grow(t_buf *b, size_t need)
{
	size_t	newcap;
	char	*p;

	newcap = b->cap;
	if (!newcap)
		newcap = 32;
	while (newcap < b->len + need + 1)
		newcap <<= 1;
	p = ft_calloc(newcap, sizeof(char));
	if (!p)
		return (1);
	if (b->data && b->len)
		ft_memcpy(p, b->data, b->len);
	free(b->data);
	b->data = p;
	b->cap = newcap;
	return (0);
}

static int	buf_pushc(t_buf *b, char c)
{
	if (b->len + 2 > b->cap)
		if (buf_grow(b, 1))
			return (1);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
	return (0);
}

static t_token	*tok_new(char *s, int type)
{
	t_token	*t;

	t = ft_calloc(1, sizeof(*t));
	if (!t)
		return (NULL);
	t->str = s;
	t->type = type;
	return (t);
}

static int	push_token(t_list **tail, char *s, int type)
{
	t_token	*tk;
	t_list	*node;

	tk = tok_new(s, type);
	if (!tk)
		return (free(s), 1);
	node = ft_calloc(1, sizeof(*node));
	if (!node)
		return (free_cmd_tok(tk), 1);
	node->content = tk;
	(*tail)->next = node;
	*tail = node;
	return (0);
}

/* ---- ALT: devuelve nuevo i. Copia contenido entre comillas a acc ---- */
static int	read_quoted_alt(const char *s, int i, char q, t_buf *acc, int *err)
{
	i++;
	while (s[i] && s[i] != q)
	{
		if (buf_pushc(acc, s[i]))
		{
			*err = 1;
			return (i);
		}
		i++;
	}
	if (s[i] == q)
		i++;
	return (i);
}

/* ---- ALT: palabra ARG fuera de comillas. Devuelve nuevo i ---- */
static int	read_word_alt(const char *s, int i, t_buf *acc, int *err)
{
	while (s[i])
	{
		if (ft_isreserved(s[i]) || ft_isspace((int)(unsigned char)s[i]))
			break ;
		if (s[i] == '\'' || s[i] == '\"')
		{
			i = read_quoted_alt(s, i, s[i], acc, err);
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

t_list	*tokenizer(char *str)
{
	t_list	head;
	t_list	*tail;
	int		i;

	head.next = NULL;
	tail = &head;
	i = 0;
	while (str && str[i])
	{
		int		start;
		char	op[3];
		int		ttype;
		t_buf	acc;
		int		err;

		i = skip_spaces_alt(str, i, &start, 1);
		if (!str[i])
			break ;
		ttype = 0;
		i = read_operator_alt(str, i, op, &ttype);
		if (ttype != 0)
		{
			char	*s;

			s = ft_strdup(op);
			if (!s || push_token(&tail, s, ttype))
				return (ft_lstclear(&head.next, free_cmd_tok), NULL);
			continue ;
		}
		acc.data = NULL;
		acc.len = 0;
		acc.cap = 0;
		err = 0;
		i = read_word_alt(str, i, &acc, &err);
		if (err)
			return (ft_lstclear(&head.next, free_cmd_tok), NULL);
		if (acc.len > 0)
		{
			char	*s;

			s = acc.data;
			if (!s || push_token(&tail, s, ARG))
				return (ft_lstclear(&head.next, free_cmd_tok), NULL);
		}
		else
			free(acc.data);
	}
	return (head.next);
}
