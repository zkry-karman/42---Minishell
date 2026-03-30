/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:13:44 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/07 16:13:48 by kzhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, s + i);
		i++;
	}
}
/*
#include <stdio.h>
#include <stdlib.h>

void	test_function(unsigned int i, char *s)
{
	unsigned int temp;

	temp = (s[0] - '0') + i;
	s[0] = temp + '0';
}

int main()
{
    char s[] = "abcdefg";

    ft_striteri(s, test_function);
    printf ("%s\n", s);
}*/
