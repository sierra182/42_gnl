/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:53:35 by seblin            #+#    #+#             */
/*   Updated: 2023/10/19 18:14:30 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>

void	del_buff(t_list *lst)
{
	t_list	*temp;
	
	while (lst)
	{
		temp = lst->next;
		if (lst->buffer)
			free(lst->buffer);
		free(lst);
		lst = temp;
	}
}

void	del_link(t_list *lst)
{
	t_list	*temp;
	
	temp = lst->next;
	del_buff(lst);
	while (lst)
	{
		
	}
}
char	**get_buffer(int fd)
{
	static t_list	*lst = NULL;
	
	if(fd == -42)
	{
		del_link(lst);
	}
	if (fd < 0)
	{
		del_buff(lst);
		lst = NULL;
		return (NULL);
	}
	while(lst)
	{
		if (lst->fd == fd)
		{		
			if (!(lst->buffer))
			{
			 	lst->buffer = (char *) ft_calloc(1, sizeof(char));				
				if (!(lst->buffer))
				{
					del_buff(lst);
					lst = NULL;
					return (NULL);
				}
			}					
			return &(lst->buffer);
		}		
		lst = lst->next; 
	}
	lst = (t_list *) ft_calloc(1, sizeof(t_list));
	if (!lst)
	{
		del_buff(lst);
		lst = NULL;
		return (NULL);
	}
	lst->fd = fd;
	lst->next = NULL;
	lst->buffer = (char *) ft_calloc(1, sizeof(char));
	if (!(lst->buffer))
	{
		del_buff(lst);
		lst = NULL;
		return (NULL);
	}
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

	buffer = NULL;	
	new_buff = NULL;
	s_chr = NULL;
	ext = NULL;
	if (BUFFER_SIZE <= 0)
		return (NULL);	 
	
	buffer = get_buffer(fd);	
	if (!buffer)
		return (NULL);
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
				free(*buffer);
				*buffer = NULL;
				return (NULL);  // liberer le fd en cours 
				// buffer = get_buffer(-1);	
				// if (!buffer)
				// 	return (NULL);
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
					free(*buffer);
					*buffer = NULL;
					return (NULL); // liberer le fd en cours 
					// buffer = get_buffer(-1);	
					// if (!buffer)
					// 	return (NULL);
				}						
				(*buffer)[0] = '\0';
				return (line);
			}
			else
			{	
				free(*buffer); // free le buffer mais pas la liste!
				*buffer = NULL;
				return (NULL);								
			}			
		}
		else 
		{
			free(ext);
			// free(*buffer); // liberer le fd en cours 
			// *buffer = NULL;
			// return (NULL);
			buffer = get_buffer(-1);	
			if (!buffer)
				return (NULL);	
		}
	}
	line = ft_strndup(*buffer, ++s_chr - *buffer);
	if (!line)
	{
		free(*buffer);
		*buffer = NULL; // liberer le fd en cours 
		return (NULL);
		//  buffer = get_buffer(-1);	
		//  if (!buffer)
		//  	return (NULL);
	}
	ft_strcpy(*buffer, s_chr);
	return (line);
}