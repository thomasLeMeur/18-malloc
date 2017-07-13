/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/06 15:26:43 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/24 16:59:15 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>
#include "ft_malloc.h"

static void	*insertblock(t_seg *s, size_t size)
{
	t_block	*b;
	t_block	*new;

	s->pos = sizeof(t_seg);
	while (s->pos < s->gbl_size)
	{
		b = (t_block *)((void *)s + s->pos);
		if (b->next - b->size - sizeof(t_block) >= size + sizeof(t_block))
		{
			if (b->size != 0)
			{
				new = (t_block *)((void *)b + sizeof(t_block) + b->size);
				new->prev = (void *)new - (void *)b;
				new->next = (void *)b + b->next - (void *)new;
				b->next = new->prev;
			}
			else
				new = b;
			new->size = size;
			return ((void *)new + sizeof(t_block));
		}
		s->pos = (s->pos + b->next <= s->pos) ? s->gbl_size : s->pos + b->next;
	}
	return (NULL);
}

static void	*insertalloc(t_seg *s, size_t size)
{
	void	*ret;

	while (s != NULL && (s->pos = sizeof(t_seg)))
	{
		if (s->gbl_size - s->alloc_size >= size + sizeof(t_block))
		{
			ret = insertblock(s, size);
			if (ret != NULL)
				return (ret);
		}
		s = s->next;
	}
	return (NULL);
}

static void	*alloc(t_seg **start, size_t size, size_t len)
{
	void	*ret;
	t_seg	*new;
	t_block	*blk;

	if ((ret = insertalloc(*start, size)) == NULL)
	{
		if ((new = mmap(NULL, len, PROT_READ | PROT_WRITE,
				MAP_SHARED | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
			return (NULL);
		new->pos = 0;
		new->next = *start;
		new->gbl_size = len;
		new->alloc_size = size + sizeof(t_seg) + sizeof(t_block);
		blk = (t_block *)((void *)new + sizeof(t_seg));
		blk->size = size;
		blk->prev = 0;
		blk->next = new->gbl_size - sizeof(t_seg);
		sortsegs(&new, NULL, NULL);
		*start = new;
		ret = (void *)blk + sizeof(t_block);
	}
	return (ret);
}

void		*malloc(size_t size)
{
	void	*ret;

	pthread_mutex_lock(&mygetenv()->locks[MALLOC_MUTEX]);
	if (size < TINY_MIN)
		ret = NULL;
	else if (size <= TINY_MAX)
		ret = alloc(&mygetenv()->tiny, size, TINY_PAGE_SIZE);
	else if (size <= SMALL_MAX)
		ret = alloc(&mygetenv()->small, size, SMALL_PAGE_SIZE);
	else if (size <= LARGE_MAX)
		ret = alloc(&mygetenv()->large, size, LARGE_PAGE_SIZE + size);
	else
		ret = NULL;
	pthread_mutex_unlock(&mygetenv()->locks[MALLOC_MUTEX]);
	return (ret);
}
