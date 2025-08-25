/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcarpio-cyepes <rcarpio-cyepes@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 18:59:25 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/25 20:23:49 by rcarpio-cye      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "structs.h"
/* Corta la línea en tokens tipados (ARG/PIPE/LOGIC/FILE_REDIR) y devuelve lista */
t_list	*tokenizer(char *str);
/* Helpers expuestos (usados por distintas fases) */
char	ft_isreserved(char c);
int		skip_spaces_alt(char *s, int i, int *start, int set_start);
int		read_operator_alt(const char *s, int i, char out[3], int *type_out);
int		state_quote_delimiter_alt(char *str, int i, char delim);
/* Post-passes */
int		ft_remove_quotes(t_list *tokens);
int		retokenize(t_list *curr, int type, int start, int *lngths);
/* Liberador estándar de t_token para usar con ft_lstclear */
void	free_cmd_tok(void *tok);
#endif
