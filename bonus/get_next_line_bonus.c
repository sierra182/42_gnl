/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:53:35 by seblin            #+#    #+#             */
/*   Updated: 2023/10/23 07:44:00 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>

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

// void	print_list(t_list *lst)
// {
// 	printf("je suis au debut de printlist!\n");
// 	while (lst)
// 	{ usleep(500000);
// 		printf("lst: %d", lst->fd);
// 		lst = lst->next;
// 	}
// 	printf("je suis a la fin de printlist!\n");
	
// }

void	del_link(t_list **link, t_list **lst)
{
	t_list	*lsttemp;
	t_list	*lstnext;
	//printf("je suis au debut de delink!\n");
	
	lsttemp = *lst;
	//lstnext = NULL;
	while (*lst)
	{
		if ((*lst)->fd == (*link)->fd)
		{
			//if ((*lst)->next)
			lstnext = (*lst)->next;
			free((*lst)->buffer);
			free(*lst);
			if (lstnext)
				*lst = lstnext;			
		}
		*lst = (*lst)->next;		
	}
	*lst = lsttemp; 
//	printf("je suis a la fin de delink!\n");
}

// void	del_link(t_list **link, t_list **lst)
// {
// 	t_list	*lsttemp;
// 	t_list	*linknext;
// 			printf("je suis au debut de delink!\n");
// 	lsttemp = *lst;
// 	if ((*link)->next)
// 	printf("EST PAS NULLEEE");
// 	else	
// 		printf("EST NULLEEE");
// 	linknext = (*link)->next;
// 	if ((*link)->buffer)
// 		free((*link)->buffer);
// 	free(*link);	
// 	*link = NULL;	
// 	if (*lst)	
// 	{
// 		while ((*lst)->next)
// 			{ 
// 				*lst = (*lst)->next;
// 			}
// 		(*lst)->next = linknext;		
// 	}
// 	else
// 		*lst = linknext; // est nulle
// 	if (linknext)	
// 	*lst = lsttemp;	// ecrasé par par lstemp!!
// 	print_list(*lst);
// 		printf("je suis a la fin de delink!\n");
// }

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
	{
	//	printf("le fd est le premier de la liste\n");
		return (lst);
	}
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
//	printf("j'ai rien trouve\n");
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
	//return (lst);
	fdlink = search_fdlink(fd, lst); // dois retourner un pointeur vers 
	if (fdlink)
	 	return (fdlink);
	//else
	//	return (NULL);
	return (lst_addfront(lst, create_newlink(fd)));
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
	if (fd < 0)
	{	
		//del_link(bufferlink, &lst);
		del_buff(&lst);		
		return (NULL);
	}
	bufferlink = get_bufferlink(fd, &lst);	
	if (!bufferlink)
		return (NULL);
//	print_list(lst);
	//printf("%d", (*bufferlink)->fd);
	//return (NULL);
	s_chr = ft_strchr((*bufferlink)->buffer, '\n');
	while (!s_chr)
	{	
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
				del_buff(&lst);	
				//del_link(bufferlink, &lst);
				return (NULL);
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
					del_buff(&lst);	
					//del_link(bufferlink, &lst);
					return (NULL);
				}						
				(*bufferlink)->buffer[0] = '\0';
				return (line);
			}
			else
			{						
				del_link(bufferlink, &lst);
				return (NULL);								
			}			
		}
		else 
		{
			free(ext);
			del_buff(&lst);
			//return (NULL);
			//del_link(bufferlink, &lst);
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
		del_buff(&lst);	
		//del_link(bufferlink, &lst);
			return (NULL);
	}
	ft_strcpy((*bufferlink)->buffer, s_chr);
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