/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:19:18 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/02 07:51:14 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

/******************************************************************************
 *
 * Ecrit la chaine de caracteres sur le file descriptor specifie en
 * determinant la longueur de la chaine
 * Si chaine = NULL, la fonction ne fait rien
 *
 *****************************************************************************/
void	ft_putstr_fd(char const *s, int fd)
{
	if (s != NULL)
		write(fd, s, ft_strlen(s));
}
