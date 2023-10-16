/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/16 16:52:38 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#define BUFF_SIZE 42

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
	char		extension[BUFF_SIZE];
	char		*new_buff;
	char	*line;
	size_t	read_size;
	char	*chr;
	
	buffer = (char *) malloc(BUFF_SIZE * sizeof (char));
	chr = NULL;
	read_size = read(fd, buffer, BUFF_SIZE - 1);
	while (read_size)
	{	
		buffer[read_size] = '\0';
		chr = ft_strchr(buffer, '\n');
		if (chr)
		{
			line = ft_strndup(buffer, chr - buffer);			
			return (line);
		}
		else
		{
			
			new_buff = ft_strjoin(buffer, extension);
			new_buff += ft_strlen(buffer);
			buffer = new_buff;
		}
	
		read_size = read(fd, buffer, ft_strlen(buffer) - 1);
	}
	if (read <= 0)
		return (NULL);
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