/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 19:53:35 by seblin            #+#    #+#             */
/*   Updated: 2023/10/19 08:14:02 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include "get_next_line.h"
#include <stdio.h>

char	**get_buffer(int fd)
{
	static t_list	*lst;
	
	while(lst)
	{
		if (lst->fd == fd)
		{
			printf("li\n");
			return &(lst->buffer);
		}		
		lst = lst->next; 
	}
	printf("la\n");
	lst = (t_list *) malloc(sizeof(t_list));
	lst->fd = fd;
	lst->buffer = (char *) ft_calloc(1, sizeof(char));
	return &(lst->buffer);
}