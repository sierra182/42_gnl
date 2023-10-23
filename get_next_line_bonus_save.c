/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:53:35 by seblin            #+#    #+#             */
/*   Updated: 2023/10/22 14:12:08 by seblin           ###   ########.fr       */
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

// void	del_link(t_list **link, t_list *lst)
// {
// 	t_list	*temp;
// 			printf("je suis au debut de delink!\n");
// 	temp = (*link)->next;
// 	if ((*link)->buffer)
// 		free((*link)->buffer);
// 	free(*link);
// 	*link = NULL;
// 	while ((lst)->next)
// 		lst = (lst)->next;
// 	if (lst)	
// 		(lst)->next = temp;	
// 		printf("je suis a la fin de delink!\n");
// }
void	print_list(t_list *lst)
{
	printf("je suis au debut de printlist!\n");
	while (lst)
	{ usleep(500000);
		printf("lst: %d", lst->fd);
		lst = lst->next;
	}
	printf("je suis a la fin de printlist!\n");
	
}

void	del_link(t_list **link, t_list **lst)
{
	t_list	*lsttemp;
	t_list	*linknext;
			printf("je suis au debut de delink!\n");
	lsttemp = *lst;
	if ((*link)->next)
	printf("EST PAS NULLEEE");
	else	
		printf("EST NULLEEE");
	linknext = (*link)->next;
	if ((*link)->buffer)
		free((*link)->buffer);
	free(*link);	
	*link = NULL;	
	if (*lst)	
	{
		while ((*lst)->next)
			{ 
				*lst = (*lst)->next;
			}
		(*lst)->next = linknext;		
	}
	else
		*lst = linknext; // est nulle
	if (linknext)	
	*lst = lsttemp;	// ecrasé par par lstemp!!
	print_list(*lst);
		printf("je suis a la fin de delink!\n");
}

t_list	**get_bufferlink(int fd, t_list **lst)
{
	t_list	*list;
	t_list	*temp;
	t_list	**ptrtemp;
	t_list	*newlink;
	printf("je suis au debt de getbuff!\n");
	//temp = (t_list *) malloc(sizeof(t_list));
	ptrtemp = (t_list **) malloc(sizeof(t_list *));
	if (!*lst)
	{printf("je ne dois apparaitre qu'une fois!\n");
		newlink = (t_list *) malloc(sizeof(t_list));
		newlink->next = NULL;
		newlink->fd = fd;
		newlink->buffer = (char *) malloc(sizeof(char));
		*lst = newlink;
		return (lst);
	}
	temp = *lst; 
	while (temp)
	{printf("je parcours la liste: %d\n", temp->fd); usleep(500000);
		if (temp->fd == fd)
		{printf("jai trouvé le fd!: %d\n", fd);
			*ptrtemp = temp;
			return (ptrtemp);
		}
		if (!temp->next)
			break ;
		temp = temp->next;		
	}	printf("je suis ici si la liste netais pas vide mais pas de fd!\n");
	newlink = (t_list *) malloc(sizeof(t_list));
	newlink->next = NULL;
	newlink->fd = fd;
	newlink->buffer = (char *) malloc(sizeof(char));

	temp->next = newlink;
	*ptrtemp = newlink;
	return (ptrtemp);
		
	// list = (t_list *) malloc(sizeof(t_list));
	// (list)->next = NULL;
	// (list)->fd = 42;
	// (list)->buffer = (char *) malloc(1 * sizeof(char));
	// t_list **retlist = (t_list **) malloc (sizeof(t_list *));
	// *retlist = list;
	// return (retlist);
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
	print_list(lst);
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
			// if (!new_buff)
			// {
			// 	// free(*buffer);
			// 	// *buffer = NULL;
			// 	// return (NULL);  // liberer le fd en cours 
			// 	// del_buff(&lst);
			// 	// return (NULL);
			// 	del_link(bufferlink, lst);
			// 	return (NULL);
			// }
			
			(*bufferlink)->buffer = new_buff;
			s_chr = ft_strchr((*bufferlink)->buffer, '\n');			
		}		
		else if (read_size == 0)
		{	
			free(ext);		
			if (*(*bufferlink)->buffer)
			{			
				line = ft_strndup((*bufferlink)->buffer, ft_strlen((*bufferlink)->buffer));
				// if (!line)
				// {
				// 	// free(*buffer);
				// 	// *buffer = NULL;
				// 	// return (NULL); // liberer le fd en cours 
				// 	// del_buff(&lst);
				// 	// return (NULL);
				// 	del_link(bufferlink, lst);
				// 	return (NULL);
				// }						
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
			//del_buff(&lst);
			//return (NULL);
			//del_link(bufferlink, lst);
			return (NULL);	
		}
	}
	line = ft_strndup((*bufferlink)->buffer, ++s_chr - (*bufferlink)->buffer);
	// if (!line)
	// {
	// 	// free(*buffer);
	// 	// *buffer = NULL; // liberer le fd en cours 
	// 	// return (NULL);
	// 	// del_buff(&lst);
	// 	// 	return (NULL);
	// 	del_link(bufferlink, lst);
	// 		return (NULL);
	// }
	ft_strcpy((*bufferlink)->buffer, s_chr);
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
