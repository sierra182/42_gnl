/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seblin <seblin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 22:43:49 by seblin            #+#    #+#             */
/*   Updated: 2023/10/26 22:45:09 by seblin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# include <stdlib.h>

typedef struct s_list
{
	int				fd;
	char			*buffer;
	struct s_list	*next;	
}					t_list;

char	*get_next_line(int fd);
void	*del_link(t_list *link, t_list **lst, t_list *lstsave);
size_t	ft_strlen(const char *s);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strchr(const char *s, int c);
void	*ft_calloc(size_t nmemb, size_t size);
char	*merge_buffers(t_list *bufferlink, char *ext, t_list **lst);

#endif