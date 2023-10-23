/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/23 09:14:12 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

char	*get_next_line(int fd)
{
	static char	*buffer;
	ssize_t		read_size;
	char		*new_buff;
	char		*s_chr;
	char		*line;
	char		*ext;
		
	new_buff = NULL;
	s_chr = NULL;
	ext = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)		
		return (free_buffer(&buffer)); 
	if (!buffer)
	{
		buffer = (char *) ft_calloc(1, sizeof(char));
		if (!buffer)
			return (NULL);				
	}
	s_chr = ft_strchr(buffer, '\n');
	while (!s_chr)
	{			

		ext = (char *) ft_calloc(BUFFER_SIZE + 1, sizeof(char));		
		read_size = read(fd, ext, BUFFER_SIZE);
		if (read_size > 0)
		{			
			new_buff = ft_strjoin(buffer, ext);
			free(buffer);
			buffer = NULL;
			free(ext);
			if (!new_buff)
				return (free_buffer(&buffer));
			
			buffer = new_buff;
			s_chr = ft_strchr(buffer, '\n');			
		}		
		else if (read_size == 0)
		{	
			free(ext);		
			if (*buffer)
			{			
				line = ft_strndup(buffer, ft_strlen(buffer));
				if (!line)
					return (free_buffer(&buffer));							
				buffer[0] = '\0';
				return (line);
			}
			else
				return (free_buffer(&buffer));			
		}
		else 
		{
			free(ext);
			return (free_buffer(&buffer));		
		}
	}
	line = ft_strndup(buffer, ++s_chr - buffer);
	if (!line)
		return (free_buffer(&buffer));
	ft_strcpy(buffer, s_chr);
	return (line);
}

/*
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
			printf("rslt: %s", rslt);
		free(rslt);
		rslt = get_next_line(fd);
	}
	close(fd);
	return (0);
}
*/



