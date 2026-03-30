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

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	unsigned const char	*dest;
	unsigned char		scr;

	dest = (unsigned const char *)s;
	scr = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (dest[i] == scr)
			return ((void *)(dest + i));
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>
int main()
{
    char *result;
    char s[30] = "sijdiaojdoa";
    result = (char *)ft_memchr(s, 'a', 6);
    printf ("%s\n", result);
}*/
