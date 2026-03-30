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

char	**ft_free_all(char **final, size_t j)
{
	while (j > 0)
	{
		j--;
		free(final[j]);
	}
	free (final);
	return (NULL);
}

size_t	word_count(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] && s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
			i++;
		}
	}
	return (count);
}

char	*hold_space(char const *s, size_t len)
{
	char	*word;
	size_t	i;

	i = 0;
	word = malloc (sizeof(char) * (len + 1));
	if (word == NULL)
		return (NULL);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char		**final;
	size_t		j;
	char const	*start;

	j = 0;
	final = ft_calloc((word_count(s, c) + 1), sizeof(char *));
	if (final == NULL)
		return (NULL);
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			start = s;
			while (*s && *s != c)
				s++;
			final[j] = hold_space (start, (s - start));
			if (final[j] == NULL)
				return (ft_free_all(final, j));
			j++;
		}
	}
	return (final);
}

/*
#include <stdio.h>
int	main()
{
	char const *s = "!!ji!!io!!!jo!!!!!!ooooo";
	int i = 0;
	int count = word_count(s, '!');
	char **result = malloc(sizeof(char *) * (count + 1));
	if (result == NULL)
		return (1);
	result = ft_split(s, '!');
	while (i < count)
	{
		printf ("%i, %s\n", i, result[i]);
		i++;
	}
	free (result);
}*/
