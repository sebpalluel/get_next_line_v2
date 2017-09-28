/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: psebasti <sebpalluel@free.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 15:07:02 by psebasti          #+#    #+#             */
/*   Updated: 2017/09/27 23:23:39 by psebasti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include "libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <stdio.h>

# define BUFF_SIZE	1

# define READ_OK	1
# define READ_EOF	0
# define READ_ERR	-1

typedef struct		s_char
{
	char			c;
	struct s_char	*next;
}					t_char;

typedef struct		s_fd
{
	t_char			*buffer;
	char			*line;
	size_t			end;
	size_t			had_line;
	int				ret;
}					t_fd;

int					get_next_line(const int fd, char **line);
#endif
