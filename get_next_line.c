/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/17 12:48:56 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1
/*
** buff zero ?
*/

static char	*ft_strcpy(char *dest, const char *src)
{
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
}

static char	*ft_strndup(const char *s, size_t n)
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

static void	*ft_free_buffer(char *buffer)
{
	free(buffer);
	return (NULL);
}

char	*ft_get_next_line(int fd)
{
	static char	*buffer;
	size_t		read_size;
	char		*new_buff;
	char		*s_chr;
	char		*line;
	char		ext[BUFFER_SIZE + 1];
	
	if (!buffer)	
		buffer = (char *) calloc(BUFFER_SIZE, sizeof(char));
	if (!buffer)
		return (NULL);	
	s_chr = ft_strchr(buffer, '\n');
	while (!s_chr)
	{			
		read_size = read(fd, ext, BUFFER_SIZE);
		if (read_size)
		{
			ext[read_size] = '\0';
			new_buff = ft_strjoin(buffer, ext);
			free(buffer);
			if (!new_buff)
				return (NULL);	
			buffer = new_buff;
			s_chr = ft_strchr(buffer, '\n');			
		}		
		if (read_size <= 0)
			if (*buffer)
			{			
				line = ft_strndup(buffer, ft_strlen(buffer));
				if (!line)
					return (ft_free_buffer(buffer));				
				buffer[0] = '\0';
				return (line);
			}
			else
				return (ft_free_buffer(buffer));			
	}
	line = ft_strndup(buffer, ++s_chr - buffer);
	if (!line)
		return (ft_free_buffer(buffer));
	ft_strcpy(buffer, s_chr);
	return (line);
}

#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int		fd;
	char	*path = "test.txt";
	char	*rslt;
	
	rslt = NULL;
	fd = open(path, O_RDONLY);
	rslt = ft_get_next_line(fd);	
	while (rslt)
	{
		if (!rslt) 
			printf("is null\n");
		else
			printf("rslt: %s", rslt);
		free(rslt);
		rslt = ft_get_next_line(fd);
	}
	return (0);
}