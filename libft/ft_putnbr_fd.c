/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:15:04 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/07 16:15:06 by kzhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putnbr_fd(int n, int fd)
{
	long int	num;
	char		c;

	num = n;
	if (num < 0)
	{
		num = -num;
		write (fd, "-", 1);
	}
	if (num > 9)
		ft_putnbr_fd(num / 10, fd);
	c = (num % 10) + '0';
	write (fd, &c, 1);
}
/*
#include <fcntl.h>

int main()
{
    int fd;

    fd = open("output_test.txt", O_WRONLY | O_CREAT, 0744);
    if (fd == -1)
        return 1;
    ft_putnbr_fd(-4464566, fd);
    close (fd);
}*/
