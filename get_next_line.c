/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psebasti <sebpalluel@free.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 14:46:00 by psebasti          #+#    #+#             */
/*   Updated: 2017/09/27 23:41:51 by psebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int			copy_buff(t_fd *fd, char *src, int ret, t_char *buffer)
{
	int				i;

	i = 0;
	while (i < ret)
	{
		if (buffer == NULL)
		{
			fd->buffer = (t_char *)ft_memalloc(sizeof(t_char));
			buffer = fd->buffer;
		}
		else
		{
			buffer->next = (t_char *)ft_memalloc(sizeof(t_char));
			buffer = buffer->next;
		}
		if (buffer == NULL)
			return (READ_ERR);
		buffer->c = src[i];
		buffer->next = NULL;
		i++;
	}
	return (READ_OK);
}

static int			read_buff(int fd, t_fd *fd_tab)
{
	t_char			*buffer;
	char			*src;
	int				ret;

	buffer = fd_tab[fd].buffer;
	while (buffer && buffer->next)
		buffer = buffer->next;
	if (!(src = ft_strnew(BUFF_SIZE)))
		return (READ_ERR);
	ret = read(fd, src, BUFF_SIZE);
	if (copy_buff(&fd_tab[fd], src, ret, buffer) < 0)
		return (READ_ERR);
	free(src);
	return (ret);
}

static void			copy_line(size_t length, t_fd *fd, char **target)
{
	char 			*line;
	t_char			*current;
	t_char			*prev;
	size_t			numchar;

	//printf("copy_line length %lu\n", length);
	line = ft_strnew(length);
	numchar = length;
	length = 0;
	current = fd->buffer;
	while (length < numchar)
	{
		line[length++] = current->c;
		prev = current;
		current = current->next;
		free(prev);
	}
	//printf("test\n");
	if (!fd->end)
	{
	//	printf("!end\n");
		prev = current;
		current = current->next;
		free(prev);
	}
	fd->buffer = current;
	*target = line;
	//printf("result %s\n", *target);
}

static int			buffer_has_line(t_fd *fd, char **target)
{
	size_t			length;
	t_char			*current;

	current = fd->buffer;
	if (current == NULL)
		*target = NULL;
	length = 0;
	printf("current %p\n", current);
	while (current != NULL && current->c != '\n' && current->c )
	{
		if (fd->end && current->next == NULL)
		{
			//		printf("break MTF\n");
			length++;
			break ;
		}
		current = current->next;
		length++;
	}
	if (current != NULL)
	{
		fd->had_line = 1;
		copy_line(length, fd, target);
		return (1);
	}
	return (0);
}

int					get_next_line(const int fd, char **line)
{
	static t_fd		fd_tab[OPEN_MAX];
	int				ret;

	ret = read(fd, NULL, 0);
	//printf("loop\n");
	if (fd < 0 || fd > OPEN_MAX || line == NULL || ret < 0)
		return (READ_ERR);
	//if (buffer_has_line(&fd_tab[fd], line))
	//{
	//	printf("last line\n");
	//	return (READ_OK);
	//}
	buffer_has_line(&fd_tab[fd], line);
	printf("end ? %lu\n", fd_tab[fd].end);
	if (fd_tab[fd].end)
		return(READ_EOF);
	fd_tab[fd].ret = 1;
	//printf("loop before while\n");
	while (!buffer_has_line(&fd_tab[fd], line) && fd_tab[fd].ret > 0)
	{
		fd_tab[fd].ret = read_buff(fd, fd_tab);
		fd_tab[fd].end = (fd_tab[fd].ret == 0) ? 1 : 0;
	//	printf("ret %d\n", fd_tab[fd].ret);
		//	printf("fd->end %lu\n", fd_tab[fd].end);
	}
	printf("end while ret %d\n", fd_tab[fd].ret);
	if (fd_tab[fd].ret < 0)
		return (READ_ERR);
	return (READ_OK);
}
