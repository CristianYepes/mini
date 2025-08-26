/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:10:46 by cristian          #+#    #+#             */
/*   Updated: 2025/08/26 17:29:28 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	push_token(t_list **tail, char *s, int type)
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

int	replace_token_head(t_token *orig, char *new_str, int type)
{
	if (!orig)
		return (1);
	if (orig->str)
		free(orig->str);
	orig->str = new_str;
	orig->type = type;
	return (0);
}
