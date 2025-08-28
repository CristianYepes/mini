/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_nodes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 00:10:46 by cristian          #+#    #+#             */
/*   Updated: 2025/08/27 20:53:50 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	push_token(t_list **tail, char *s, int type)
{
	t_list	*node;
	t_token	*tk;

	tk = (t_token *)ft_calloc(1, sizeof(*tk));
	if (!tk)
		return (free(s), 1);
	tk->str = s;
	tk->type = type;
	node = (t_list *)ft_calloc(1, sizeof(*node));
	if (!node)
		return (free_cmd_tok(tk), 1);
	node->content = tk;
	(*tail)->next = node;
	*tail = node;
	return (0);
}

void	tok_set_quoted_by(t_list *tail, char q)
{
	t_token	*tk;

	if (!tail)
		return ;
	tk = (t_token *)tail->content;
	if (tk)
		tk->quoted_by = q;
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
