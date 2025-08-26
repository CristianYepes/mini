/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquote_pass.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:31:03 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/26 17:28:23 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unquote_copy(const char *in, int i, char q, t_buf *out)
{
	i++;
	while (in[i] && in[i] != q)
	{
		if (buf_pushc(out, in[i]))
			return (-1);
		i++;
	}
	if (in[i] == q)
		i++;
	return (i);
}

static char	*str_unquote_dup(const char *in)
{
	t_buf	out;
	int		i;

	buf_init(&out);
	i = 0;
	while (in && in[i])
	{
		if (in[i] == '\'' || in[i] == '\"')
		{
			i = unquote_copy(in, i, in[i], &out);
			if (i < 0)
				return (free(out.data), NULL);
		}
		else
		{
			if (buf_pushc(&out, in[i]))
				return (free(out.data), NULL);
			i++;
		}
	}
	return (buf_steal(&out));
}

int	ft_remove_quotes(t_list *tokens)
{
	while (tokens)
	{
		t_token	*tk;
		char	*newstr;

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
	if (!str || str[i] != delim)
		return (i);
	i++;
	while (str[i] && str[i] != delim)
		i++;
	if (str[i] == delim)
		i++;
	return (i);
}

