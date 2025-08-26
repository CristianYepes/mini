/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_buf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:08:11 by cristian          #+#    #+#             */
/*   Updated: 2025/08/26 15:36:04 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	buf_init(t_buf *b)
{
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
}

int	buf_pushc(t_buf *b, char c)
{
	if (b->len + 2 > b->cap)
		if (buf_grow(b, 1))
			return (1);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
	return (0);
}

char	*buf_steal(t_buf *b)
{
	char	*s;

	s = b->data;
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
	return (s);
}

int	vec_grow(char ***vec, int *cap, int pieces)
{
	char	**nv;
	int		k;

	nv = ft_calloc((*cap << 1) + 1, sizeof(char *));
	if (!nv)
		return (1);
	k = 0;
	while (k < pieces)
	{
		nv[k] = (*vec)[k];
		k++;
	}
	free(*vec);
	*vec = nv;
	*cap <<= 1;
	return (0);
}
