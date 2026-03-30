/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclean.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:39:30 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/14 18:08:11 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*current;
	t_list	*sus;

	if (!lst)
		return ;
	current = *lst;
	while (current != NULL)
	{
		sus = current->next;
		ft_lstdelone (current, del);
		current = sus;
	}
	*lst = NULL;
}
/*
#include <stdio.h>

void	del(void *content)
{
	free (content);	
}

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

char	*ft_strdup(const char *s1)
{
	char	*ptr;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (s1[len])
		len++;
	ptr = malloc (sizeof(char) * (len + 1));
	if (ptr == NULL)
		return (NULL);
	while (i < len)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

int main()
{
    char *node1 = ft_strdup("I will be freed");
	char *node2 = ft_strdup("me too");

    t_list *my_node1 = ft_lstnew(node1);
	t_list *my_node2 = ft_lstnew(node2);
	ft_lstadd_front (&my_node1, my_node2);
    printf("Node created.\n");
    ft_lstclear(&my_node2, del);
    printf("Node deleted.\n");
    return 0;
}*/
