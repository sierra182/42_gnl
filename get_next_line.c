/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/17 10:31:56 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 200

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
	s2 = (char *) ft_calloc(size + 1, sizeof (char));
	if (!s2)
		return (NULL);
	ft_strlcpy(s2, s, size + 1);
	return (s2);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		ext[BUFFER_SIZE + 1];
	char		*new_buff;
	char	*line;
	size_t	read_size;
	char	*chr;
	
	if (!buffer)
	{
		buffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
		buffer[BUFFER_SIZE - 1] = '\0';				
	}
	chr = ft_strchr(buffer, '\n');
	while (!chr)
	{			
		read_size = read(fd, ext, BUFFER_SIZE - 1);
		if (read_size)
		{
			ext[read_size] = '\0';
			new_buff = ft_strjoin(buffer, ext);
			buffer = new_buff;
			chr = ft_strchr(buffer, '\n');			
		}		
		if (read_size <= 0)
			if (*buffer)
			{
				line = buffer;
				buffer = ft_strchr(buffer, '\0');
				return (line);
			}
			else
				return (NULL);
			
	}
	line = ft_strndup(buffer, chr++ - buffer);
	ft_strcpy(buffer, chr);
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
	rslt = get_next_line(fd);	
	while (rslt)
	{
		if (!rslt) 
			printf("is null\n");
		else
			printf("rslt: %s\n", rslt);
		rslt = get_next_line(fd);
	}
	return (0);
}