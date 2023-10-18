/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/18 17:25:57 by svidot           ###   ########.fr       */
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
	{	
		if (buffer)		
			free(buffer);
		buffer = NULL;
		return (NULL);	 
	}	 			
	if (!buffer)
	{
		buffer = (char *) ft_calloc(1, sizeof(char));
		if (!buffer)
			return (NULL);				
	}
	s_chr = ft_strchr(buffer, '\n');
	while (!s_chr)
	{	
			// if (ft_strlen(buffer) > SIZE_MAX - (BUFFER_SIZE + 1))
			// {	
						
			// 	line = ft_strndup(buffer, ft_strlen(buffer));
			// 	if (!line)
			// 	{
			// 		free(buffer);
			// 		buffer = NULL;
			// 		return (NULL);
			// 	}
			// 	free(buffer);
			// 	buffer = NULL;
			// 	return (line);	
			// }
		ext = (char *) ft_calloc(BUFFER_SIZE + 1, sizeof(char));		
		read_size = read(fd, ext, BUFFER_SIZE);
		if (read_size > 0)
		{			
			new_buff = ft_strjoin(buffer, ext);
			free(buffer);
			buffer = NULL;
			free(ext);
			if (!new_buff)
			{
				free(buffer);
				buffer = NULL;
				return (NULL);
			}
			
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
				{
					free(buffer);
					buffer = NULL;
					return (NULL);
				}				
				buffer[0] = '\0';
				return (line);
			}
			else
			{				
				//return (ft_free_buffer(buffer));
				free(buffer);
				buffer = NULL;
				return (NULL);
				//return (buffer);					
			}			
		}
		else //if (read_size < 0)
		{
			free(ext);
			free(buffer);
			buffer = NULL;
			return (NULL);
		//	return (ft_free_buffer(buffer));
		}
	}
	line = ft_strndup(buffer, ++s_chr - buffer);
	if (!line)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	ft_strcpy(buffer, s_chr);
	return (line);
}
/*

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
	close(fd);
	return (0);
}
*/






