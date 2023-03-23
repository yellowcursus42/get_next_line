/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skulkamt <skulkamt@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 17:16:14 by skulkamt          #+#    #+#             */
/*   Updated: 2023/03/23 18:18:16 by skulkamt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <string.h>

size_t	max(size_t a, size_t b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

void	mayalloc(t_vec *a)
{
	char	*newone;
	size_t	new_size;

	if (a->buff == NULL || a->offset + BUFFER_SIZE >= a->size)
	{
		new_size = max(a->size * 2, BUFFER_SIZE) + 1;
		newone = malloc(new_size);
		if (newone)
		{
			newone[0] = 0;
			if (a->buff)
				ft_memcpy(newone, a->buff, a->offset + 1);
			a->size = new_size;
		}
		else
		{
			a->size = 0;
			a->offset = 0;
		}
		free(a->buff);
		a->buff = newone;
	}
}

ssize_t	get_next_string(int fd, char *read_buf)
{
	ssize_t	totalread;

	totalread = read(fd, read_buf, BUFFER_SIZE);
	if (totalread >= 0)
		read_buf[totalread] = '\0';
	return (totalread);
}

char	*cont(t_vec *vec, int fd)
{
	char	*ret;
	ssize_t	readsize;

	mayalloc(vec);
	if (!vec->buff)
		return (NULL);
	readsize = get_next_string(fd, vec->buff + vec->offset);
	if (readsize < 0)
		return (NULL);
	else if (readsize == 0)
	{
		if (vec->offset > 0)
		{
			ret = ft_strdup(vec->buff);
			vec->offset = 0;
			return (ret);
		}
		else
			return (NULL);
	}
	else
	{
		vec->offset += readsize;
		return (get_next_line(fd));
	}
}

char	*get_next_line(int fd)
{
	static t_vec	vec;
	char			*next_end;
	size_t			retsize;
	char			*ret;

	next_end = NULL;
	if (vec.buff != NULL)
		next_end = ft_strchr(vec.buff, '\n');
	if (next_end != NULL)
	{
		retsize = next_end - vec.buff + 1;
		ret = malloc(retsize + 1);
		if (ret)
		{
			ft_memcpy(ret, vec.buff, retsize);
			ret[retsize] = 0;
			ft_memcpy(vec.buff, vec.buff + retsize, vec.size - retsize);
			vec.offset -= retsize;
		}
		return (ret);
	}
	return (cont(&vec, fd));
}
