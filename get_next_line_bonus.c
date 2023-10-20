/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:53:35 by seblin            #+#    #+#             */
/*   Updated: 2023/10/20 10:29:47 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>

void	del_buff(t_list **lst)
{
	t_list	*temp;
	//t_list	*start;

	//start = *lst;
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->buffer)
			free((*lst)->buffer);
		free(*lst);
		*lst = temp;
	}
	//*lst = start;
}

void	del_link(t_list **link, t_list *lst)
{
	t_list	*temp;
	
	temp = (*link)->next;
	free(*link);
	*link = NULL;
	while (lst->next)
		lst = lst->next;
	lst->next = temp;
}

t_list	**get_bufferlink(int fd, t_list **lst)
{
	t_list			*temp;

	temp = *lst;	
	
	while(*lst)
	{
		if ((*lst)->fd == fd)
		{		
			if (!(*lst)->buffer)
			{
			 	(*lst)->buffer = (char *) ft_calloc(1, sizeof(char));				
				if (!((*lst)->buffer))
				{
					// del_buff(&temp);
					// lst = NULL;
					// return (NULL);
				}
			}	
			t_list **bufferlink = lst;
			if (temp)
			*lst = temp;
			return (bufferlink);		
			//return (&(*lst)->buffer);
		}		
		*lst = (*lst)->next; 
	}
	*lst = (t_list *) ft_calloc(1, sizeof(t_list));
	if (!lst)
	{
		// del_buff(lst);
		// lst = NULL;
		// return (NULL);
	}
	(*lst)->fd = fd;
	(*lst)->next = NULL;
	(*lst)->buffer = (char *) ft_calloc(1, sizeof(char));
	if (!((*lst)->buffer))
	{
		// del_buff(&temp);
		// lst = NULL;
		// return (NULL);
	}
	t_list **bufferlink = lst;
	if (temp)
		*lst = temp;
	return (bufferlink);
	//return(&lst->buffer);
}

char	*get_next_line(int fd)
{
	static t_list	*lst = NULL;
	t_list		**bufferlink;
	ssize_t		read_size;
	char		*new_buff;
	char		*s_chr;
	char		*line;
	char		*ext;

	bufferlink = NULL;	
	new_buff = NULL;
	s_chr = NULL;
	ext = NULL;
	if (BUFFER_SIZE <= 0)
		return (NULL);	 
	// if (fd < 0)
	// {	del_buff(&lst);		
	// 	return (NULL);
	// }
	bufferlink = get_bufferlink(fd, &lst);	
	if (!bufferlink)
		return (NULL);

	s_chr = ft_strchr((*bufferlink)->buffer, '\n');
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
			new_buff = ft_strjoin((*bufferlink)->buffer, ext);
			free((*bufferlink)->buffer);
			(*bufferlink)->buffer = NULL;
			free(ext);
			if (!new_buff)
			{
				// free(*buffer);
				// *buffer = NULL;
				// return (NULL);  // liberer le fd en cours 
				// del_buff(&lst);
				// return (NULL);
			}
			
			(*bufferlink)->buffer = new_buff;
			s_chr = ft_strchr((*bufferlink)->buffer, '\n');			
		}		
		else if (read_size == 0)
		{	
			free(ext);		
			if (*(*bufferlink)->buffer)
			{			
				line = ft_strndup((*bufferlink)->buffer, ft_strlen((*bufferlink)->buffer));
				if (!line)
				{
					// free(*buffer);
					// *buffer = NULL;
					// return (NULL); // liberer le fd en cours 
					// del_buff(&lst);
					// return (NULL);
				}						
				(*bufferlink)->buffer[0] = '\0';
				return (line);
			}
			else
			{	
				// free(*buffer); // free le buffer mais pas la liste!
				// *buffer = NULL;
				// return (NULL);	
				
				del_buff(&lst);
				return (NULL);								
			}			
		}
		else 
		{
			free(ext);
			// free(*buffer); // liberer le fd en cours 
			// *buffer = NULL;
			// return (NULL);
			
			del_buff(&lst);
			//return (NULL);
		//del_link(bufferlink, lst);
			return (NULL);	
		}
	}
	line = ft_strndup((*bufferlink)->buffer, ++s_chr - (*bufferlink)->buffer);
	if (!line)
	{
		// free(*buffer);
		// *buffer = NULL; // liberer le fd en cours 
		// return (NULL);
		// del_buff(&lst);
		// 	return (NULL);
	}
	ft_strcpy((*bufferlink)->buffer, s_chr);
	return (line);
}