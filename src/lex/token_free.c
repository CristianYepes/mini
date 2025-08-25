/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcarpio-cyepes <rcarpio-cyepes@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 21:25:19 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/25 20:26:41 by rcarpio-cye      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_cmd_tok(void *tok)
{
	t_token	*t;

	t = tok;
	if (!t)
		return ;
	if (t->str)
		free(t->str);
	free(t);
}
