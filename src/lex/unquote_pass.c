/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unquote_pass.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:31:03 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/27 20:50:11 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_wrapped(const char *s, char q)
{
	size_t	i;

	if (!s || s[0] != q)
		return (0);
	i = 1;
	while (s[i] && s[i] != q)
		i++;
	if (!s[i] || s[i + 1] != '\0')
		return (0);
	return (1);
}

static void	strip_outer(char *s)
{
	size_t	i;
	size_t	j;

	i = 1;
	j = 0;
	while (s[i] && s[i + 1])
	{
		s[j] = s[i];
		i++;
		j++;
	}
	s[j] = '\0';
}

int	ft_remove_quotes(t_list *tokens)
{
	t_token	*tk;

	while (tokens)
	{
		tk = (t_token *)tokens->content;
		if (tk && tk->str)
		{
			if (tk->quoted_by == '\'' && is_wrapped(tk->str, '\''))
				strip_outer(tk->str);
			else if (tk->quoted_by == '\"' && is_wrapped(tk->str, '\"'))
				strip_outer(tk->str);
		}
		tokens = tokens->next;
	}
	return (0);
}

