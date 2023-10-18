/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/18 11:57:08 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int is_fd_valid(int fd) {
    return fcntl(fd, F_GETFD) != -1 || errno == EBADF;
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	size_t		read_size;
	char		*new_buff;
	char		*s_chr;
	char		*line;
	char		ext[BUFFER_SIZE + 1];
	
	 if (!is_fd_valid(fd))
	 	return (ft_free_buffer(buffer));
	
	 int i = 0;
	 while (i < BUFFER_SIZE + 1)
	 	ext[i++] = 0;
	if (!buffer)
	{
		buffer = (char *) ft_calloc(BUFFER_SIZE, sizeof(char));
		if (!buffer)
			return (NULL);
		//buffer = (char *) malloc(sizeof(char));//	BUFFER_SIZE	
		// if (buffer)
		// 	*buffer = 0;
		// else
		// 	return (NULL);		
	}		
	s_chr = ft_strchr(buffer, '\n');
	while (!s_chr)
	{			
		read_size = read(fd, ext, BUFFER_SIZE);
		if (read_size > 0)
		{
			ext[read_size] = '\0';
			new_buff = ft_strjoin(buffer, ext);
			free(buffer);
			if (!new_buff)
				return (NULL);	
			buffer = new_buff;
			s_chr = ft_strchr(buffer, '\n');			
		}		
		else if (read_size == 0)
		{			
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
	char	*path = "empty.txt";
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
	return (0);
}







