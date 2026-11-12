/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <lucas@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 08:16:29 by lurodri2          #+#    #+#             */
/*   Updated: 2026/11/12 09:32:53 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*helper_read(int fd, char *str, char *buff)
{
	int		n;
	int		t;

	t = ft_strchar(str);
	n = 1;
	while (n > 0 && t == 0)
	{
		n = read(fd, buff, BUFFER_SIZE);
		if (n > 0)
		{
			buff[n] = '\0';
			str = ft_strjoin(str, buff);
			if (!str)
				n = -1;
			t = ft_strchar(buff);
		}
	}
	if (n == -1)
	{
		free(str);
		return (NULL);
	}
	return (str);
}

char	*read_for_stash(int fd, char *str)
{
	char	*buff;

	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
	{
		free(str);
		return (NULL);
	}
	str = helper_read(fd, str, buff);
	free(buff);
	return (str);
}

char	*ext_line(char *str)
{
	char	*s;
	int		i;
	int		j;

	if (!str || !str[0])
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	s = malloc(i + 1);
	if (!s)
		return (NULL);
	while (j < i)
	{
		s[j] = str[j];
		j++;
	}
	s[j] = '\0';
	return (s);
}

char	*upd_line(char *str)
{
	char	*s;
	int		i;

	if (!str || !str[0])
	{
		free(str);
		return (NULL);
	}
	i = helper(str);
	if (str[i] != '\n')
	{
		free(str);
		return (NULL);
	}
	if (str[i + 1] == '\0')
	{
		free(str);
		return (NULL);
	}
	s = ft_strdup(&str[i + 1]);
	free(str);
	return (s);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_for_stash(fd, stash);
	if (!stash)
		return (NULL);
	line = ext_line(stash);
	stash = upd_line(stash);
	return (line);
}
/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "get_next_line_bonus.h"

int	main(void)
{
	char	*files[3] = {"a.txt", "b.txt", "c.txt"};
	int		fd[3];
	char	*line;
	int		i;
	int		abertos;

	i = 0;
	while (i < 3)
	{
		fd[i] = open(files[i], O_RDONLY);
		i++;
	}
	abertos = 3;
	while (abertos > 0)
	{
		i = 0;
		while (i < 3)
		{
			if (fd[i] != -1)
			{
				line = get_next_line(fd[i]);
				if (line)
				{
					printf("[%s] %s", files[i], line);
					if (line[0] && line[strlen(line) - 1] != '\n')
						printf("\n");
					free(line);
				}
				else
				{
					printf("[%s] fim (NULL)\n", files[i]);
					close(fd[i]);
					fd[i] = -1;
					abertos--;
				}
			}
			i++;
		}
	}
	return (0);
}*/