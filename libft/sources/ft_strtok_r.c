/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok_r.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 09:12:36 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/02 07:55:02 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <stdlib.h>

char	*ft_strtok_r(char *str, const char *delim, char **saveptr)
{
	char	*token;

	if (str != NULL)
	{
		*saveptr = str;
	}
	if (*saveptr == NULL || **saveptr == '\0')
	{
		return (NULL);
	}
	token = *saveptr;
	*saveptr = ft_strpbrk(token, delim);
	if (*saveptr != NULL)
	{
		*(*saveptr)++ = '\0';
	}
	return (token);
}
