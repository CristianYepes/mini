/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_retokenize.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 20:26:34 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/26 16:30:27 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	scan_word(const char *in, int i, t_buf *acc)
{
	while (in[i] && !ft_isspace((int)(unsigned char)in[i]))
	{
		if (in[i] == '\'' || in[i] == '\"')
		{
			char	q = in[i++];

			while (in[i] && in[i] != q)
				if (buf_pushc(acc, in[i++]))
					return (-1);
			if (in[i] == q)
				i++;
			continue ;
		}
		if (buf_pushc(acc, in[i++]))
			return (-1);
	}
	return (i);
}

static int	append_piece(const char *in, int *i, char ***vec, int *pieces, int *cap)
{
	t_buf	acc;
	int		j;

	buf_init(&acc);
	j = scan_word(in, *i, &acc);
	if (j < 0)
		return (free(acc.data), -1);
	*i = j;
	if (acc.len == 0)
		return (free(acc.data), 0);
	if (*pieces == *cap && vec_grow(vec, cap, *pieces))
		return (free(acc.data), -1);
	(*vec)[(*pieces)++] = buf_steal(&acc);
	return (1);
}

static int	split_outside_quotes(const char *in, char ***out_vec)
{
	char	**vec;
	int		i;
	int		pieces;
	int		cap;
	int		st;

	vec = ft_calloc(9, sizeof(char *));
	if (!vec)
		return (-1);
	i = 0;
	pieces = 0;
	cap = 8;
	while (in && in[i])
	{
		i = skip_spaces_alt((char *)in, i, NULL, 0);
		if (!in[i])
			break ;
		st = append_piece(in, &i, &vec, &pieces, &cap);
		if (st < 0)
			return (free(vec), -1);
	}
	*out_vec = vec;
	return (pieces);
}

static int	attach_and_finish(t_list **pcurr, char **parts, int n, int type, t_list *after)
{
	int	i;

	i = 1;
	while (i < n)
	{
		if (push_token(pcurr, parts[i], type))
			return (1);
		i++;
	}
	free(parts);
	(*pcurr)->next = after;
	return (0);
}

int	retokenize(t_list *curr, int type, int start, int *lngths)
{
	t_token	*orig;
	char	*slice, **parts;
	int		n;

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
	free(slice);
	if (n <= 0)
		return (n < 0);
	if (replace_token_head(orig, parts[0], type))
		return (0);
	if (attach_and_finish(&curr, parts, n, type, curr->next))
		return (0);
	return (0);
}
