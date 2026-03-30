/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:15:04 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/16 15:13:51 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst != NULL)
	{
		(*f)(lst->content);
		lst = lst->next;
	}
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

void	test_function(void *content)
{
	printf("Content of this node is: %s\n", (char *)content);
}

void	del(void *content)
{
	free (content);	
}

void	ft_lstdelone(t_list *lst, void(*del)(void*))
{
	if (!lst)
		return ;
	if (del)
		(*del)(lst->content);
	free (lst);
}

void    ft_lstclear(t_list **lst, void (*del)(void*))
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

int main()
{
	t_list *head = NULL;
	char *node1 = ft_strdup("i'm node 1");
	char *node2 = ft_strdup("i'm node too");
    t_list *my_node1 = ft_lstnew(node1);
	t_list *my_node2 = ft_lstnew(node2);
	ft_lstadd_front (&head, my_node1);
	ft_lstadd_front (&head, my_node2);
	ft_lstiter (head, test_function);
	ft_lstclear(&head, del);
}*/