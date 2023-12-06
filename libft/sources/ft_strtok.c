/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/29 14:04:08 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/02 07:55:08 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

int	is_delimiter(char c, const char *delimiters)
{
	while (*delimiters != '\0')
	{
		if (c == *delimiters)
		{
			return (1);
		}
		delimiters++;
	}
	return (0);
}

char	*ft_strtok(char *str, const char *delimiters)
{
	static char	*next_token;
	char		*start;

	if (str == NULL)
	{
		if (next_token == NULL)
		{
			return (NULL);
		}
		str = next_token;
	}
	start = str;
	while (*str != '\0')
	{
		if (is_delimiter(*str, delimiters))
		{
			*str = '\0';
			next_token = str + 1;
			return (start);
		}
		str++;
	}
	next_token = NULL;
	return (start);
}
