/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:53:35 by seblin            #+#    #+#             */
/*   Updated: 2023/10/19 13:27:55 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>

char	**get_buffer(int fd)
{
	static t_list	*lst;
	
	while(lst)
	{
		if (lst->fd == fd)
		{		
			if (!(lst->buffer))	
				lst->buffer = (char *) ft_calloc(1, sizeof(char));	
			return &(lst->buffer);
		}		
		lst = lst->next; 
	}
	lst = (t_list *) malloc(sizeof(t_list));
	lst->fd = fd;
	lst->buffer = (char *) ft_calloc(1, sizeof(char));
	return &(lst->buffer);
}

char	*get_next_line(int fd)
{
	char		**buffer;
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
		//if (buffer)		
			//free(buffer);
		buffer = NULL;
		return (NULL);	 
	}	
		
	buffer = get_buffer(fd);	
	
	// if (!buffer)
	// {
	// 	buffer = (char *) ft_calloc(1, sizeof(char));
	// 	if (!buffer)
	// 		return (NULL);				
	// }
	s_chr = ft_strchr(*buffer, '\n');
	while (!s_chr)
	{	
			// printf("NUM: %zu\n", ft_strlen(buffer));
			// if (ft_strlen(buffer) > SIZE_MAX - (BUFFER_SIZE + 2))
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
			new_buff = ft_strjoin(*buffer, ext);
			free(*buffer);
			*buffer = NULL;
			free(ext);
			if (!new_buff)
			{
				free(buffer);
				buffer = NULL;
				return (NULL);
			}
			
			*buffer = new_buff;
			s_chr = ft_strchr(*buffer, '\n');			
		}		
		else if (read_size == 0)
		{	
			free(ext);		
			if (**buffer)
			{			
				line = ft_strndup(*buffer, ft_strlen(*buffer));
				if (!line)
				{
					free(buffer);
					buffer = NULL;
					return (NULL);
				}						
				(*buffer)[0] = '\0';
				return (line);
			}
			else
			{	
				free(*buffer);
				*buffer = NULL;
				return (NULL);								
			}			
		}
		else 
		{
			free(ext);
			free(*buffer);
			*buffer = NULL;
			return (NULL);		
		}
	}
	line = ft_strndup(*buffer, ++s_chr - *buffer);
	if (!line)
	{
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	ft_strcpy(*buffer, s_chr);
	return (line);
}