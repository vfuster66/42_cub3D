/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 09:10:08 by vfuster-          #+#    #+#             */
/*   Updated: 2023/11/20 10:03:38 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	const char	*hay;
	const char	*ned;

	if (*needle == '\0')
		return ((char *)haystack);
	while (*haystack != '\0')
	{
		hay = haystack;
		ned = needle;
		while (*ned != '\0' && *hay == *ned)
		{
			hay++;
			ned++;
		}
		if (*ned == '\0')
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
