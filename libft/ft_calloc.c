/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:39:30 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/10 11:40:36 by kzhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void			*ptr;
	unsigned char	*final_ptr;
	size_t			i;
	size_t			total_size;

	i = 0;
	total_size = count * size;
	if (size != 0 && total_size / size != count)
		return (NULL);
	ptr = malloc (total_size);
	if (ptr == NULL)
		return (NULL);
	final_ptr = (unsigned char *)ptr;
	while (i < total_size)
	{
		final_ptr[i] = 0;
		i++;
	}
	return ((void *)final_ptr);
}

/*
int main(void)
{
    size_t count;
    size_t size;
    size_t total;mea
    unsigned char *ptr;
    size_t i;

    count = 5;
    size = 2;
    total = count * size;
    ptr = ft_calloc(count, size);
    if (ptr == NULL)
    {
        printf("ft_calloc: NULL\n");
        return 1;
    }

    i = 0;
    while (i < total)
    {
        if (ptr[i] != 0)
        {
            printf("fail at %zu\n", i);
            free(ptr);
            return 1;
        }
        i++;
    }

    printf("ok\n");
    free(ptr);
    return 0;
}*/
