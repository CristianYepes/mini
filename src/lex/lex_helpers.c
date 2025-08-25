/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcarpio-cyepes <rcarpio-cyepes@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 03:50:39 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/25 20:26:12 by rcarpio-cye      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	ft_isreserved(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '<' || c == '>');
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
	type = 0;
	if (!s || !s[i] || !ft_isreserved(s[i]))
	{
		*type_out = 0;
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


