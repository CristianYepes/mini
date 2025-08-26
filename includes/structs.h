/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cristian <cristian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 18:45:28 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/26 18:02:47 by cristian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stddef.h>

/* Tipos de token */
# define FILE_REDIR 0
# define PIPE       1
# define LOGIC      2
# define ARG        3

/* Token */
typedef struct s_token
{
	char	*str;
	int		type;
}	t_token;

/* Buffer dinámico usado por el lexer */
typedef struct s_buf
{
	char	*data;
	size_t	len;
	size_t	cap;
}	t_buf;

/* Paquete para lectura con comillas (evita +4 args) */
typedef struct s_scan
{
	t_buf	*acc;
	int		*err;
}	t_scan;

#endif
