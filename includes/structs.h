/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcarpio-cyepes <rcarpio-cyepes@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 18:45:28 by rcarpio-cye       #+#    #+#             */
/*   Updated: 2025/08/23 18:57:09 by rcarpio-cye      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# define FILE_REDIR	0 /* <, >, <<, >> */
# define PIPE		1 /* | */
# define LOGIC		2 /* &&, ||, ; */
# define ARG		3 /* palabra/argumento normal */

//-*************-Estructura de cada token-***************-
typedef struct s_token
{
	char	*str;	/* texto del token*/
	int		type;	/* unas de las macros de arriba*/
}	t_token;

#endif
