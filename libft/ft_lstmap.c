/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cocozhu <cocozhu@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:12:33 by kzhu              #+#    #+#             */
/*   Updated: 2025/11/16 15:13:48 by cocozhu          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_node;
	void	*new_content;

	if (!lst || !f || !del)
		return (NULL);
	new_lst = NULL;
	while (lst != NULL)
	{
		new_content = (f)(lst->content);
		if (new_content)
			new_node = ft_lstnew(new_content);
		if (!new_content || !new_node)
		{
			if (new_content)
				del(new_content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new_node);
		lst = lst->next;
	}
	return (new_lst);
}
/*
#include <stdio.h>

void	del(void *content)
{
	free (content);	
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

void	*test_function(void *content)
{
	char *str = (char *)content;
	char *new_str;
	int i = 0;

	new_str = ft_strdup(str);
    if (new_str == NULL)
		return (NULL);
	while (new_str[i])
	{
		if (new_str[i] >= 'a' && new_str[i] <= 'z')
        {
            new_str[i] = new_str[i] - 32;
        }
        i++;
	}
	return (new_str);
}

void	to_print(void *content)
{
	printf("Content of this node is: %s\n", (char *)content);
}

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

int main()
{
	t_list *head = NULL;
	t_list *new_lst;
	char *node1 = ft_strdup("i'm node 1");
	char *node2 = ft_strdup("i'm node too");
	char *node3 = ft_strdup("i'm node three");
    t_list *my_node1 = ft_lstnew(node1);
	t_list *my_node2 = ft_lstnew(node2);
	t_list *my_node3 = ft_lstnew(node3);
	ft_lstadd_back (&head, my_node1);
	ft_lstadd_back (&head, my_node2);
	ft_lstadd_back (&head, my_node3);
	new_lst = ft_lstmap(head, test_function, del);
	ft_lstiter(new_lst, to_print);
	ft_lstclear(&head, del);
	ft_lstclear(&new_lst, del);
}*/
