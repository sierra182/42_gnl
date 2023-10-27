/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: svidot <svidot@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 13:29:55 by svidot            #+#    #+#             */
/*   Updated: 2023/10/27 11:16:02 by svidot           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>

void	*del_link(t_list *link, t_list **lst, t_list *lstsave)
{
	t_list	*lstnext;

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
			*lst = lstsave;
			break ;
		}
		*lst = (*lst)->next;
	}
	return (NULL);
}

static t_list	*get_bufferlink(int fd, t_list **lst)
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

static char	*manage_endfile(t_list *bufferlink, char *ext, t_list **lst)
{
	char		*line;

	free(ext);
	if (*bufferlink->buffer)
	{
		line = ft_strndup(bufferlink->buffer, ft_strlen(bufferlink->buffer));
		if (!line)
			return (del_link(bufferlink, lst, *lst));
		bufferlink->buffer[0] = '\0';
		return (line);
	}
	else
		return (del_link(bufferlink, lst, *lst));
}

static char	*manage_no_newline(int fd, t_list *bufferlink,
	t_list **lst, char **newline)
{
	char		*ext;
	ssize_t		read_size;

	while (!*newline)
	{
		ext = (char *) ft_calloc(BUFFER_SIZE + 1, sizeof(char));
		if (!ext)
			return (del_link(bufferlink, lst, *lst));
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
			return (del_link(bufferlink, lst, *lst));
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
	char			*buffersave;

	bufferlink = get_bufferlink(fd, &lst);
	if (!bufferlink)
		return (NULL);
	newline = ft_strchr(bufferlink->buffer, '\n');
	if (!newline)
	{
		line = manage_no_newline(fd, bufferlink, &lst, &newline);
		if ((!line && !newline) || line)
			return (line);
	}
	line = ft_strndup(bufferlink->buffer, ++newline - bufferlink->buffer);
	if (!line)
		return (del_link(bufferlink, &lst, lst));
	buffersave = bufferlink->buffer;
	while (*newline)
		*bufferlink->buffer++ = *newline++;
	*bufferlink->buffer = '\0';
	bufferlink->buffer -= bufferlink->buffer - buffersave;
	return (line);
}
