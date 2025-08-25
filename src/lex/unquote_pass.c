/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquote_pass.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcarpio-cyepes <rcarpio-cyepes@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:31:03 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/25 20:26:47 by rcarpio-cye      ###   ########.fr       */
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

static char	*str_unquote_dup(const char *in)
{
	t_buf	out;
	int		i;

	out.data = NULL;
	out.len = 0;
	out.cap = 0;
	i = 0;
	while (in && in[i])
	{
		if (in[i] == '\'' || in[i] == '\"')
		{
			i++;
			while (in[i] && in[i] != in[i - 1])
			{
				if (buf_pushc(&out, in[i]))
					return (free(out.data), NULL);
				i++;
			}
			if (in[i])
				i++;
		}
		else
		{
			if (buf_pushc(&out, in[i]))
				return (free(out.data), NULL);
			i++;
		}
	}
	return (out.data);
}

int	ft_remove_quotes(t_list *tokens)
{
	t_token	*tk;
	char	*newstr;

	while (tokens)
	{
		tk = (t_token *)tokens->content;
		if (tk && tk->str)
		{
			newstr = str_unquote_dup(tk->str);
			if (newstr)
			{
				free(tk->str);
				tk->str = newstr;
			}
		}
		tokens = tokens->next;
	}
	return (0);
}

int	state_quote_delimiter_alt(char *str, int i, char delim)
{
	if (!str || !str[i] || str[i] != delim)
		return (i);
	i++;
	while (str[i] && str[i] != delim)
		i++;
	if (str[i] == delim)
		i++;
	return (i);
}
