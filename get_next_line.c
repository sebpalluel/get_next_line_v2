/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psebasti <sebpalluel@free.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 14:46:00 by psebasti          #+#    #+#             */
/*   Updated: 2017/09/28 21:46:06 by psebasti         ###   ########.fr       */
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
	if (!fd_tab[fd].has_line && ret < BUFF_SIZE)
	{
		fd_tab[fd].has_line = ft_strcountchar(src, '\n') + 1;
		if (ret == 0 && fd_tab[fd].has_line == 1)
			fd_tab[fd].end = 1;
//		printf("ret %d has_line %d\n",ret, fd_tab[fd].has_line);
	}
	free(src);
	return (ret);
}

static void			copy_line(size_t length, t_fd *fd, char **target)
{
	char 			*line;
	t_char			*current;
	t_char			*prev;
	size_t			numchar;

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
	if (!fd->end)
	{
		prev = current;
		current = current->next;
		free(prev);
	}
	fd->buffer = current;
	*target = line;
}

static int			buffer_has_line(t_fd *fd, char **target)
{
	size_t			length;
	t_char			*current;

	current = fd->buffer;
	if (current == NULL)
		*target = NULL;
	length = 0;
	while (current != NULL && current->c != '\n')
	{
		current = current->next;
		if (fd->has_line == 1 && current->next == NULL)
			break ;
		length++;
	}
	if (current != NULL || (fd->end && length > 0))
	{
		if (fd->has_line)
		{
			fd->has_line--;
			//printf("fd->has_line %lu\n", fd->has_line);
		}
		if (fd->ret == 0 && fd->has_line == 0)
		{
			fd->end = 1;
			//printf("fd->end %lu\n", fd->end);
		}
		copy_line(length, fd, target);
		return (1);
	}
	return (0);
}

int					get_next_line(const int fd, char **line)
{
	static t_fd		fd_tab[OPEN_MAX] = {{NULL, 0, 0, 0}};

//	printf("loop\n");
	if (fd < 0 || fd > OPEN_MAX || line == NULL || read(fd, NULL, 0))
		return (READ_ERR);
	fd_tab[fd].ret = 1;
	while (fd_tab[fd].ret && !buffer_has_line(&fd_tab[fd], line))
	{
		fd_tab[fd].ret = read_buff(fd, fd_tab);
//	printf("end %lu, ret %d, has_line %d\n", fd_tab[fd].end, fd_tab[fd].ret, fd_tab[fd].has_line);
	}
	if (fd_tab[fd].has_line == 1)
	{
	if (buffer_has_line(&fd_tab[fd], line))
		return(READ_OK);
	}
	if (fd_tab[fd].end || (fd_tab[fd].ret == 0 && fd_tab[fd].has_line == 1))
		return(READ_EOF);
	if (fd_tab[fd].ret < 0)
		return (READ_ERR);
	//if (!buffer_has_line(&fd_tab[fd], line))
	//	return (READ_EOF);
	return (READ_OK);
}
