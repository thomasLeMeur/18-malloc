/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   others.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/06 15:26:43 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/24 16:59:49 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"
#include <sys/mman.h>

void	free(void *ptr)
{
	t_seg	*s;
	t_block	*b;

	pthread_mutex_lock(&mygetenv()->locks[FREE_MUTEX]);
	if ((s = findseg(ptr)) != NULL)
	{
		b = (t_block *)((void *)s + s->pos);
		s->alloc_size -= b->size + sizeof(t_block);
		b->size = 0;
		if (s->pos > sizeof(t_seg))
		{
			((t_block *)(void *)b - b->prev)->next += b->next;
			b->next = 0;
			b->prev = 0;
		}
	}
	pthread_mutex_unlock(&mygetenv()->locks[FREE_MUTEX]);
}

void	*realloc(void *ptr, size_t size)
{
	t_seg	*s;
	t_block	*b;
	void	*tmp;

	pthread_mutex_lock(&mygetenv()->locks[REALLOC_MUTEX]);
	tmp = NULL;
	if (ptr == NULL)
		tmp = malloc(size);
	else if (size >= TINY_MIN && size <= LARGE_MAX
			&& (s = findseg(ptr)) != NULL)
	{
		tmp = ptr;
		b = (t_block *)((void *)s + s->pos);
		if (b->size >= size)
			b->size = size;
		else if ((tmp = malloc(size)) != NULL)
		{
			ft_memcpy(tmp, ptr, size);
			free(ptr);
		}
	}
	pthread_mutex_unlock(&mygetenv()->locks[REALLOC_MUTEX]);
	return (tmp);
}

void	*calloc(size_t nmemb, size_t size)
{
	void	*tmp;

	pthread_mutex_lock(&mygetenv()->locks[CALLOC_MUTEX]);
	tmp = NULL;
	if (nmemb && size)
	{
		tmp = malloc(nmemb * size);
		ft_bzero(tmp, nmemb * size);
	}
	pthread_mutex_unlock(&mygetenv()->locks[CALLOC_MUTEX]);
	return (tmp);
}
