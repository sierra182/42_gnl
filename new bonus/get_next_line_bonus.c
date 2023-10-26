/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/26 19:41:31 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>

#include <stdio.h>

// void	print_list(t_list *lst)
// {
// 	printf("je suis au debut de printlist!\n");
// 	while (lst)
// 	{ 
// 		usleep(500000);
// 		printf("lst: %d\n", lst->fd);
// 		lst = lst->next;
// 	}
// 	printf("je suis a la fin de printlist!\n");	
// }

void	del_link(t_list *link, t_list **lst)
{
	t_list	*lsttemp;
	t_list	*lstnext;

	lsttemp = *lst;
	while (*lst)
	{
		if ((*lst)->fd == link->fd)
		{
			lstnext = (*lst)->next;
			free((*lst)->buffer);
			free(*lst);
			*lst = lstnext;
			break ;
		}
		else if ((*lst)->next->fd == link->fd)
		{
			lstnext = (*lst)->next->next;
			free((*lst)->next->buffer);
			free((*lst)->next);
			(*lst)->next = lstnext;
			*lst = lsttemp;
			break ;
		}
		*lst = (*lst)->next;
	}	
}

// t_list	*create_newlink(int fd)
// {
// 	t_list	*newlink;

// 	newlink = (t_list *) ft_calloc(1, sizeof(t_list));
// 	if (!newlink)
// 		return (NULL);
// 	newlink->fd = fd;
// 	newlink->buffer = (char *) ft_calloc(1, sizeof(char));
// 	if (!newlink->buffer)
// 	{
// 		free(newlink);
// 		newlink = NULL;
// 		return (NULL);	
// 	}
// 	newlink->next = NULL;
// 	return (newlink);
// }

// t_list	*get_bufferlink(int fd, t_list **lst)
// {
// 	t_list	*fdlink;
// 	t_list	*temp;

// 	if (!*lst)
// 	{		
// 		*lst = create_newlink(fd);	
// 		if (!*lst)
// 			return (NULL);
// 	}
// 	if ((*lst)->fd == fd)
// 		return (*lst);		
// 	temp = *lst;
// 	while (temp->next)
// 	{	
// 		if (temp->next->fd == fd)
// 		{	
// 			fdlink = temp->next;		
// 			return (fdlink);
// 		}
// 		temp = temp->next;
// 	}
// 	fdlink = create_newlink(fd);
// 	if (!fdlink)
// 		return (NULL);
// 	fdlink->next = *lst;
// 	*lst = fdlink;	
// 	return (*lst);
// }

// t_list	*get_bufferlink(int fd, t_list **lst)
// {
// 	t_list	*fdlink;
// 	t_list	*temp;

// 	temp = *lst;	
// 	while (*lst && temp && temp->fd != fd)
// 		temp = temp->next;	
// 	if (temp)		
// 		return (temp);		
// 	fdlink = create_newlink(fd);
// 	if (!fdlink)
// 		return (NULL);
// 	fdlink->next = *lst;
// 	*lst = fdlink;
// 	return (*lst);
// }

t_list	*get_bufferlink(int fd, t_list **lst)
{
	t_list	*fdlink;
	t_list	*temp;

	temp = *lst;	
	while (*lst && temp && temp->fd != fd)
		temp = temp->next;	
	if (temp)		
		return (temp);		
	fdlink = (t_list *) ft_calloc(1, sizeof(t_list));
	if (!fdlink)
		return (NULL);
	fdlink->fd = fd;
	fdlink->buffer = (char *) ft_calloc(1, sizeof(char));
	if (!fdlink->buffer)
	{
		free(fdlink);		
		return (NULL);	
	}
	fdlink->next = *lst;
	*lst = fdlink;
	return (*lst);
}

char	*manage_endfile(t_list *bufferlink, char *ext, t_list **lst)
{
	char		*line;

	free(ext);
	if (*bufferlink->buffer)
	{	
		line = ft_strndup(bufferlink->buffer, ft_strlen(bufferlink->buffer));
		if (!line)
		{
			del_link(bufferlink, lst);		
			return (NULL);
		}		
		bufferlink->buffer[0] = '\0'; 
		return (line);
	}
	else 
	{
		del_link(bufferlink, lst);		
		return (NULL);		
	}
}

// char	*manage_prequel( char **buffer)
// {
// 	//if (fd < 0 || BUFFER_SIZE <= 0)
// 		//return (NULL);
// 		//return (free_buffer(buffer));
// 	if (!*buffer)
// 	{
// 		*buffer = (char *) ft_calloc(1, sizeof(char));
// 		// if (!*buffer)
// 		// 	return (NULL);
// 	}
// 	return ("ok");
// }

char	*merge_buffers(t_list *bufferlink, char *ext, t_list **lst)
{
	char	*new_buff;
	size_t	buffer_len;
	size_t	ext_len;

	buffer_len = ft_strlen(bufferlink->buffer);
	ext_len = ft_strlen(ext);
	new_buff = (char *) malloc((buffer_len + ext_len + 1) * sizeof (char));
	if (!new_buff)
	{
		free(ext);
		del_link(bufferlink, lst);		
		return (NULL);
	}
	while (*bufferlink->buffer)
		*new_buff++ = *bufferlink->buffer++;
	while (*ext)
		*new_buff++ = *ext++;
	*new_buff = '\0';
	bufferlink->buffer -= buffer_len;
	free(bufferlink->buffer);
	free(ext - ext_len);
	bufferlink->buffer = new_buff - ext_len - buffer_len;
	return ("ok");
}

char	*manage_no_newline(int fd, t_list *bufferlink, t_list **lst, char **newline)
{
	char		*ext;
	ssize_t		read_size;

	while (!*newline)
	{
		ext = (char *) ft_calloc(BUFFER_SIZE + 1, sizeof(char));
		if (!ext)
		{
			del_link(bufferlink, lst);		
			return (NULL);
		}		
		read_size = read(fd, ext, BUFFER_SIZE);
		if (read_size > 0)
		{
			if (!merge_buffers(bufferlink, ext, lst))
				return (NULL);
			*newline = ft_strchr(bufferlink->buffer, '\n');
		}
		else if (read_size == 0)
			return (manage_endfile(bufferlink, ext, lst));
		else
		{
			free(ext);
			del_link(bufferlink, lst);		
			return (NULL);			
		}
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static t_list	*lst = NULL;
	t_list			*bufferlink;	
	char			*newline;
	char			*line;
	char			*rslt_nonewl;
	size_t			newline_len;

	bufferlink = get_bufferlink(fd, &lst);
	if (!bufferlink)
		return (NULL);
	newline = ft_strchr(bufferlink->buffer, '\n');
	if (!newline)
	{
		rslt_nonewl = manage_no_newline(fd, bufferlink, &lst, &newline);
		if ((!rslt_nonewl && !newline) || rslt_nonewl)
			return (rslt_nonewl);
	}
	line = ft_strndup(bufferlink->buffer, ++newline - bufferlink->buffer);
	if (!line)
	{
		del_link(bufferlink, &lst);		
		return (NULL);
	}	
	newline_len = ft_strlen(newline);
	while (*newline)
		*bufferlink->buffer++ = *newline++;
	*bufferlink->buffer = '\0';
	bufferlink->buffer -= newline_len;
	return (line);
}

/*
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

*/
