/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_emit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 17:51:14 by cristian          #+#    #+#             */
/*   Updated: 2025/08/26 17:51:45 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Emite operador si lo hay; did = 1 si se emitió */
int	emit_operator_if_any(const char *s, int i, t_list **tail, int *did)
{
	char	op[3];
	int		ttype;
	char	*dup;

	*did = 0;
	ttype = ARG;
	i = read_operator_alt(s, i, op, &ttype);
	if (ttype == ARG)
		return (i);
	dup = ft_strdup(op);
	if (!dup || push_token(tail, dup, ttype))
		return (-1);
	*did = 1;
	return (i);
}

/* Emite un argumento vacío "" como ARG */
int	emit_empty_arg(t_list **tail)
{
	char	*dup;

	dup = ft_strdup("");
	if (!dup || push_token(tail, dup, ARG))
		return (1);
	return (0);
}
