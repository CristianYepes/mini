/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_retokenize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcarpio-cyepes <rcarpio-cyepes@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 20:26:34 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/25 20:26:36 by rcarpio-cye      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct s_buf
{
	char	*data;
	size_t	len;
	size_t	cap;
}	t_buf;

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

static int	split_outside_quotes(const char *in, char ***out_vec)
{
	t_buf	acc;
	char	**vec;
	int		i;
	int		pieces;
	int		cap;

	acc.data = NULL;
	acc.len = 0;
	acc.cap = 0;
	vec = ft_calloc(9, sizeof(char *));
	if (!vec)
		return (-1);
	i = 0;
	pieces = 0;
	cap = 8;
	while (in && in[i])
	{
		while (in[i] && ft_isspace((int)(unsigned char)in[i]))
			i++;
		if (!in[i])
			break ;
		acc.len = 0;
		while (in[i] && !ft_isspace((int)(unsigned char)in[i]))
		{
			if (in[i] == '\'' || in[i] == '\"')
			{
				i++;
				while (in[i] && in[i] != in[i - 1])
				{
					if (buf_pushc(&acc, in[i]))
						return (free(acc.data), -1);
					i++;
				}
				if (in[i])
					i++;
			}
			else
			{
				if (buf_pushc(&acc, in[i]))
					return (free(acc.data), -1);
				i++;
			}
		}
		if (acc.len > 0)
		{
			if (pieces == cap)
			{
				char	**nv;
				int		k;

				nv = ft_calloc((cap << 1) + 1, sizeof(char *));
				if (!nv)
					return (free(acc.data), -1);
				k = 0;
				while (k < pieces)
				{
					nv[k] = vec[k];
					k++;
				}
				free(vec);
				vec = nv;
				cap <<= 1;
			}
			vec[pieces] = acc.data;
			pieces++;
			acc.data = NULL;
		}
	}
	*out_vec = vec;
	return (pieces);
}

int	retokenize(t_list *curr, int type, int start, int *lngths)
{
	t_list	*after;
	t_token	*orig;
	char	*slice;
	char	**parts;
	int		n;
	int		i;

	(void)lngths;
	if (!curr || !curr->content)
		return (1);
	orig = (t_token *)curr->content;
	if (!orig->str || start < 0 || (size_t)start > ft_strlen(orig->str))
		return (1);
	slice = ft_substr(orig->str, start, ft_strlen(orig->str) - start);
	if (!slice)
		return (1);
	n = split_outside_quotes(slice, &parts);
	if (n <= 0)
		return (free(slice), (n < 0));
	free(orig->str);
	orig->str = parts[0];
	orig->type = type;
	after = curr->next;
	i = 1;
	while (i < n)
	{
		t_token	*tk;
		t_list	*nd;

		tk = ft_calloc(1, sizeof(*tk));
		nd = ft_calloc(1, sizeof(*nd));
		if (!tk || !nd)
			return (free(tk), free(nd), 0);
		tk->str = parts[i];
		tk->type = type;
		nd->content = tk;
		nd->next = NULL;
		curr->next = nd;
		curr = nd;
		i++;
	}
	free(parts);
	free(slice);
	curr->next = after;
	return (0);
}
