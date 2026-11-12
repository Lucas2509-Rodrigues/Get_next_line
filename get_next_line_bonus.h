/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <lucas@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 07:33:17 by lurodri2          #+#    #+#             */
/*   Updated: 2026/11/12 09:32:41 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# ifndef LIMIT_FD
#  define LIMIT_FD 1024
# endif

int		ft_strlen(char *str);
int		ft_strchar(char *str);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strdup(const char *s);
int		helper(char *str);
char	*get_next_line(int fd);
#endif