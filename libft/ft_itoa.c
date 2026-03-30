/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:39:00 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/05 14:39:29 by kzhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

long int	count_digits(long int n)
{
	long int	count;

	count = 0;
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

void	write_digits(char *result, long int n, long int count)
{
	int	pos;

	pos = count - 1;
	if (n == 0)
	{
		result[pos] = '0';
		return ;
	}
	while (n > 0)
	{
		result[pos] = (n % 10) + '0';
		n = n / 10;
		pos--;
	}
}

char	*ft_itoa(int n)
{
	long int	number;
	long int	count;
	char		*result;
	int		if_negative;

	number = n;
	if_negative = 1;
	count = 0;
	if (number <= 0)
	{
		if (number < 0)
			if_negative = -1;
		number = -number;
		count++;
	}
	count += count_digits(number);
	result = malloc(sizeof(char) * (count + 1));
	if (result == NULL)
		return (NULL);
	result[count] = '\0';
	write_digits(result, number, count);
	if (if_negative < 0)
		result[0] = '-';
	return (result);
}

/*
#include <stdio.h>
int main()
{
    int n = -545489489;
    printf ("%s", ft_itoa(n));
}*/
