/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 17:18:16 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/02 07:50:38 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

/******************************************************************************
 *
 * Permet d'ecrire un caractere sur un file descriptor
 * Utilise write qui permet d'ecrire des donnees dans un file descriptor
 *
 *****************************************************************************/
void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
