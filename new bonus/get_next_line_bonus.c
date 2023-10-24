/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/24 13:28:51 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include <stdio.h>
void	del_buff(t_list **lst)
{
	t_list	*temp;

	while (*lst)
	{
		temp = (*lst)->next;
		free((*lst)->buffer);		
		free(*lst);
		*lst = temp;
	}
}

void	del_link(t_list **link, t_list **lst)
{
	t_list	*lsttemp;
	t_list	*lstnext;

	
	lsttemp = *lst;
	while (*lst)
	{
		if ((*lst)->fd == (*link)->fd)
		{			
			lstnext = (*lst)->next;
			free((*lst)->buffer);
			free(*lst);
			if (lstnext)
				*lst = lstnext;			
		}
		*lst = (*lst)->next;		
	}
	*lst = lsttemp; 
}

t_list	*create_newlink(int fd)
{
	t_list	*newlink;

	newlink = (t_list *) malloc(sizeof(t_list));
	newlink->fd = fd;
	newlink->buffer = (char *) malloc(sizeof(char));	
	newlink->next = NULL;
	return (newlink);
}

t_list	**search_fdlink(int fd, t_list **lst)
{
	t_list	**fdlink;
	t_list	*temp;
	
	if ((*lst)->fd == fd)
		return (lst);	
	fdlink = (t_list **) malloc(sizeof(t_list *));
	temp = *lst;
	while (temp->next)
	{	
		*fdlink = temp;
		if (temp->next->fd == fd)
		{	*fdlink = temp->next;		
			return (fdlink);
		}
		temp = temp->next;
	}
	return (NULL);
}

t_list	**lst_addfront(t_list **lst, t_list *new)
{
	new->next = *lst;
	*lst = new;
	return (lst);
}

t_list	**get_bufferlink(int fd, t_list **lst)
{
	t_list	**fdlink;
	
	if (!*lst)
		*lst = create_newlink(fd); 	
	fdlink = search_fdlink(fd, lst); 
	if (fdlink)
	 	return (fdlink);	
	return (lst_addfront(lst, create_newlink(fd)));
}

char	*manage_endfile(char **buffer, char *ext)
{
	char		*line;

	free(ext);
	if (**buffer)
	{
		line = ft_strndup(*buffer, ft_strlen(*buffer));
		// if (!line)
		// 	return (free_buffer(buffer));
		(*buffer)[0] = '\0';
		return (line);
	}
	else		
		return (free_buffer(buffer));
}

char	*manage_prequel( char **buffer)
{
	//if (fd < 0 || BUFFER_SIZE <= 0)
		//return (NULL);
		//return (free_buffer(buffer));
	if (!*buffer)
	{
		*buffer = (char *) ft_calloc(1, sizeof(char));
		// if (!*buffer)
		// 	return (NULL);
	}
	return ("ok");
}

char	*merge_buffers(char **buffer, char *ext)
{
	char	*new_buff;
	size_t	buffer_len;
	size_t	ext_len;

	buffer_len = ft_strlen(*buffer);
	ext_len = ft_strlen(ext);
	new_buff = (char *) malloc((buffer_len + ext_len + 1) * sizeof (char));
	// if (!new_buff)
	// {
	// 	free(ext);
	// 	return (free_buffer(buffer));
	// }
	while (**buffer)
		*new_buff++ = *(*buffer)++;
	while (*ext)
		*new_buff++ = *ext++;
	*new_buff = '\0';
	*buffer -= buffer_len;
	free(*buffer);
	free(ext - ext_len);
	*buffer = new_buff - ext_len - buffer_len;
	return ("ok");
}

char	*manage_no_newline(int fd, char **buffer, char **newline)
{
	char		*ext;
	ssize_t		read_size;

	while (!*newline)
	{
		ext = (char *) ft_calloc(BUFFER_SIZE + 1, sizeof(char));
		// if (!ext)
		// 	return (free_buffer(buffer));
		read_size = read(fd, ext, BUFFER_SIZE);
		if (read_size > 0)
		{
			if (!merge_buffers(buffer, ext))
				return (NULL);
			*newline = ft_strchr(*buffer, '\n');
		}
		else if (read_size == 0)
			return (manage_endfile(buffer, ext));
		else
		{
			free(ext);
			return (free_buffer(buffer));			
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
	size_t		newline_len;

	// if (!manage_prequel(&buffer))
	// 	return (NULL);
	buffer =
	newline = ft_strchr(buffer, '\n');
	if (!newline)
	{
		rslt_nonewl = manage_no_newline(fd, &buffer, &newline);
		if ((!rslt_nonewl && !newline) || rslt_nonewl)
			return (rslt_nonewl);
	}
	line = ft_strndup(buffer, ++newline - buffer);
	// if (!line)
	// 	return (free_buffer(&buffer));
	newline_len = ft_strlen(newline);
	while (*newline)
		*buffer++ = *newline++;
	*buffer = '\0';
	buffer -= newline_len;
	return (line);
}

#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int		fd;
	int		fd2;
	char	*path = "test.txt";
	char	*path2 = "test2.txt";
	char	*rslt;
	char	*rslt2;
	
	rslt = "a";
	rslt2 = "a";
	fd = open(path, O_RDONLY);
	fd2 = open(path2, O_RDONLY);
		
	int pass = 0;	
	while (rslt)
	{
		rslt = get_next_line(fd);
		if (!rslt) 
			printf("is null\n");
		else
			printf("rslt: %s", rslt);
		free(rslt);
		
		printf("\033[36m");
		while (!pass && rslt2)
		{
			rslt2 = get_next_line(fd2);	
			if (!rslt2) 
			printf("is null\n");
			else
			printf("in rslt: %s", rslt2);
			free(rslt2);	
		}
		pass++;
		printf("\033[0m");
	}
	close(fd);
	fd = open(path, O_RDONLY);
	rslt = "a";
	printf("\033[33m");
	while (rslt)
	{
		rslt = get_next_line(fd);
		if (!rslt) 
			printf("is null\n");
		else
			printf("rslt: %s", rslt);
		free(rslt);
	}
	printf("\033[0m");
	close(fd);
	close(fd2);
	return (0);
}

