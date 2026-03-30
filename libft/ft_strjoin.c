/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:39:30 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/10 11:40:36 by kzhu             ###   ########.fr       */
/*                                                                    */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*final;
	size_t	i;
	size_t	j;
	size_t	total_len;

	i = 0;
	j = 0;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2);
	final = malloc (sizeof(char) * (total_len + 1));
	if (final == NULL)
		return (NULL);
	while (s1[i])
	{
		final[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		final[i + j] = s2[j];
		j++;
	}
	final[i + j] = '\0';
	return (final);
}
/*
#include <stdio.h>
int main()
{
    char *s;
    char *s1 = "@                   !";
    char *s2 = "42";

    s = ft_strjoin(s1, s2);
    printf ("%s\n", s);
    free (s);
}*/
