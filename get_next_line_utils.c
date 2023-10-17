/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 14:14:05 by svidot            #+#    #+#             */
/*   Updated: 2023/10/17 14:32:45 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <string.h>
#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
	return (dest);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*s2;
	size_t	s_len;
	size_t	size;

	s_len = ft_strlen(s);
	if (n > s_len)
		size = s_len;
	else
		size = n;
	s2 = (char *) malloc((size + 1) * sizeof (char));
	if (!s2)
		return (NULL);
	ft_strlcpy(s2, s, size + 1);
	return (s2);
}

void	*ft_free_buffer(char *buffer)
{
	free(buffer);
	return (NULL);
}