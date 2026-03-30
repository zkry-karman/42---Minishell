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

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}
/*
#include <stdio.h>

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

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

void	print_list(t_list *head)
{
	while (head != NULL)
	{
		printf("%s->", (char *)head->content);
		head = head->next;
	}
	printf("NULL\n");
}

int	main()
{
	t_list	*head;
	t_list	*node1;
	t_list	*node2;
	t_list	*node3;

	head = NULL;
	node1 = ft_lstnew("world!");
	ft_lstadd_front(&head, node1);
	node2 = ft_lstnew("Hello");
	ft_lstadd_front(&head, node2);
	node3 = ft_lstnew("42");
	ft_lstadd_front(&head, node3);
	print_list(head);
	print_list(ft_lstlast(head));
}*/