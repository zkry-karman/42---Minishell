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

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t				i;
	unsigned const char	*des;
	unsigned const char	*src;

	des = (unsigned const char *)s1;
	src = (unsigned const char *)s2;
	i = 0;
	while (i < n)
	{
		if (des[i] != src[i] || des[i] == '\0')
			return (des[i] - src[i]);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
int main()
{
    const char *s1 = "ABCDE";
    const char *s2 = "ABBBBBB";
    printf ("%i\n", ft_strncmp(s1, s2, 3));
}*/