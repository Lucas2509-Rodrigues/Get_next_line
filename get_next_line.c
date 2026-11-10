/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lurodri2 <lurodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 08:16:29 by lurodri2          #+#    #+#             */
/*   Updated: 2026/11/10 12:44:07 by lurodri2         ###   ########.fr       */
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
/*int main (void)
{
	int	fd;
	char	*linha;
	fd = open("teste.txt", O_RDONLY);
	linha = get_next_line(fd);
	while (linha)
	{
		printf("%s", linha);      // sem \n extra: a linha já o traz
		free(linha);              // cada linha é tua para libertar
		linha = get_next_line(fd);
	}
	close(fd);
	return (0);
}*/