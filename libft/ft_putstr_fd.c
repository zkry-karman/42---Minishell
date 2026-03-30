/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kzhu <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:14:12 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/07 16:14:15 by kzhu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		write (fd, &s[i], 1);
		i++;
	}
}
/*
#include <fcntl.h>

int main()
{
    char *s = "hi, 42!";
    int fd;

    fd = open("output_test.txt", O_WRONLY | O_CREAT, 0744);
    if (fd == -1)
        return 1;
    ft_putstr_fd(s, fd);
    close (fd);
}*/
