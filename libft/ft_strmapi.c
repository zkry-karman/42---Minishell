/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:12:33 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/07 16:12:40 by kzhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*result;
	int		len;
	int		i;

	len = 0;
	i = 0;
	if (s == NULL || f == NULL)
		return (NULL);
	while (s[len])
		len++;
	result = malloc(sizeof(char) * (len + 1));
	if (result == NULL)
		return (NULL);
	while (i < len)
	{
		result[i] = f(i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}
/*
#include <stdio.h>

char test_function(unsigned int i, char c)
{
    unsigned int temp;
    char new;

    temp = (c - '0') + i;
    new = temp + '0';
    return (new);
}

int main()
{
    char const *s = "abcdefg";
    char *result;

    result = ft_strmapi(s, &test_function);
    printf ("%s\n", result);
    free (result);
}*/
