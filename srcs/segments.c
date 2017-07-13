/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segments.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 11:49:19 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/08 17:43:38 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

static int	findptr(t_seg *s, size_t offset, void *ptr)
{
	t_block *b1;
	t_block *b2;

	if (ptr > (void *)s && ptr < (void *)s + offset)
	{
		if (ptr < (void *)s + sizeof(t_seg) + sizeof(t_block))
			return (0);
		b1 = (t_block *)(ptr - sizeof(t_block));
		if ((void *)b1 - b1->prev <= (void *)s ||
				(void *)b1 - b1->prev > (void *)b1)
			return (0);
		b2 = (t_block *)(b1 - b1->prev);
		if (b2 != b1 && (void *)b2 + b2->next <= (void *)b1)
			return (0);
		s->pos = ptr - sizeof(t_block) - (void *)s;
		return (1);
	}
	return (0);
}

t_seg		*findseg(void *ptr)
{
	t_env	*e;
	t_seg	*s;
	int		cnt;
	size_t	offset;

	if (!ptr)
		return (NULL);
	cnt = 0;
	e = mygetenv();
	s = e->tiny;
	offset = TINY_PAGE_SIZE;
	while (cnt++ < 3)
	{
		while (s != NULL)
			if (findptr(s, offset, ptr) == 1)
				return (s);
			else
				s = s->next;
		s = (cnt == 1) ? e->small : e->large;
		if (cnt == 1)
			offset = SMALL_PAGE_SIZE;
		else
			offset = (s == NULL) ? 0 : s->gbl_size;
	}
	return (NULL);
}

void		sortsegs(t_seg **s, t_seg *pa, t_seg *pb)
{
	t_seg	*a;
	t_seg	*b;
	t_seg	*nextb;

	a = *s;
	pa = NULL;
	while (a != NULL && (b = a->next))
	{
		pb = NULL;
		while (b != NULL)
		{
			if (b < a && ((nextb = b->next) || 1))
			{
				*s = (*s == a) ? b : *s;
				(pa != NULL) ? (pa->next = b) : 0;
				b->next = (pb != NULL) ? (pb->next -= pb->next - a)->next : a;
				a->next = nextb;
				nextb = a;
				a = b;
				b = nextb;
			}
			b = (pb -= pb - b)->next;
		}
		a = (pa -= pa - a)->next;
	}
}
