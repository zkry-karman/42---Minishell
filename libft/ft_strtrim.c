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

size_t	ft_tofind(const char *set, char c)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*final;
	size_t	len;
	size_t	i;
	size_t	j;

	i = 0;
	len = 0;
	j = 0;
	if (s1 == NULL || set == NULL)
		return (NULL);
	while (s1[len])
		len++;
	while (ft_tofind(set, s1[i]) == 1 && s1[i])
		i++;
	while (ft_tofind(set, s1[len - 1]) == 1 && len > i)
		len--;
	final = malloc (sizeof(char) * (len - i + 1));
	if (final == NULL)
		return (NULL);
	while (i < len)
		final[j++] = s1[i++];
	final[j] = '\0';
	return (final);
}
/*
#include <stdio.h>
int main ()
{
    char *s = "!!!12345!!";
    char *set ="!@";
    char *final;
    final = ft_strtrim(s, set);
    printf ("%s\n", final);
    free (final);
}*/