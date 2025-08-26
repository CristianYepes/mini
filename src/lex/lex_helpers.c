/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 23:54:40 by cristian          #+#    #+#             */
/*   Updated: 2025/08/26 17:28:38 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	ft_isreserved(char c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	skip_spaces_alt(char *s, int i, int *start, int set_start)
{
	if (!s)
		return (i);
	while (s[i] && ft_isspace((int)(unsigned char)s[i]))
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
	return (ARG);
}

int	read_operator_alt(const char *s, int i, char out[3], int *type_out)
{
	int	type;

	out[0] = '\0';
	out[1] = '\0';
	out[2] = '\0';
	if (!s || !s[i] || !ft_isreserved(s[i]))
		return (*type_out = ARG, i);
	out[0] = s[i++];
	if (s[i] && s[i] == out[0]
		&& (out[0] == '>' || out[0] == '<' || out[0] == '|' || out[0] == '&'))
		out[1] = s[i++];
	type = op_type(out);
	*type_out = type;
	return (i);
}

int	lex_handle_op(const char *s, int i, t_list **tail)
{
	int	did;
	int	nxt;

	did = 0;
	nxt = emit_operator_if_any(s, i, tail, &did);
	if (nxt < 0)
		return (-1);
	if (did)
		return (nxt);
	return (i);
}
