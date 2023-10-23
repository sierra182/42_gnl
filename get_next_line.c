/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/23 12:48:47 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

char*	manage_endfile(char **buffer, char* ext)
{
	char		*line;
	
	free(ext);		
	if (**buffer)
	{			
		line = ft_strndup(*buffer, ft_strlen(*buffer));
		if (!line)
			return (free_buffer(buffer));							
		(*buffer)[0] = '\0';
		return (line);
	}
	else
		return (free_buffer(buffer));	
}

char	*manage_prequel(int fd, char **buffer)
{
	if (fd < 0 || BUFFER_SIZE <= 0)		
		return (free_buffer(buffer)); 
	if (!*buffer)
	{
		*buffer = (char *) ft_calloc(1, sizeof(char));
		if (!*buffer)
			return (NULL);				
	}
	return ("ok");	
}

char	*manage_no_newline(int fd, char **buffer, char **newline)
{
	char		*ext;
	char		*new_buff;
	ssize_t		read_size;
	
	while (!*newline)
	{		
		ext = (char *) ft_calloc(BUFFER_SIZE + 1, sizeof(char));		
		read_size = read(fd, ext, BUFFER_SIZE);
		if (read_size > 0)
		{			
			new_buff = ft_strjoin(*buffer, ext);
			free(*buffer);
			*buffer = NULL;
			free(ext);
			if (!new_buff)
				return (free_buffer(buffer));			
			*buffer = new_buff;
			*newline = ft_strchr(*buffer, '\n');			// null ou reassigne newline
		}		
		else if (read_size == 0)
			return (manage_endfile(buffer, ext)); // null ou line
		else 
		{
			free(ext);
			return (free_buffer(buffer));		// null
		}
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*buffer;	
	char		*newline;
	char		*line;
	char		*rslt_nonewl;

	if (!manage_prequel(fd, &buffer))
		return (NULL);
	newline = ft_strchr(buffer, '\n');
	if (!newline)
	{
		rslt_nonewl = manage_no_newline(fd, &buffer, &newline);
		if (!rslt_nonewl && !newline)
			return (rslt_nonewl);
	}	
	line = ft_strndup(buffer, ++newline - buffer);
	if (!line)
		return (free_buffer(&buffer));
	ft_strcpy(buffer, newline);
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



