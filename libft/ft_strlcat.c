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

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	i;

	i = 0;
	src_len = 0;
	dst_len = 0;
	while (src[src_len])
		src_len++;
	while (dst_len < dstsize && dst[dst_len])
		dst_len++;
	if (dstsize == 0 || dst_len >= dstsize)
		return (src_len + dst_len);
	while ((dst_len + i) < (dstsize - 1) && src[i])
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (src_len + dst_len);
}
/*
#include <stdio.h>
int main()
{
    char dst[30] = "hello ";
    const char *src = "peppa pig";
    printf ("%zu\n", ft_strlcat(dst, src, 20));
    printf ("%s\n", dst);
}*/
