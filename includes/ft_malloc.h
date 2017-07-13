/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/06 15:23:14 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/24 17:04:23 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include <pthread.h>

# define B					PTHREAD_MUTEX_INITIALIZER
# define FREE_MUTEX			(0)
# define CALLOC_MUTEX		(1)
# define MALLOC_MUTEX		(2)
# define REALLOC_MUTEX		(3)
# define SHOWMEM_MUTEX		(4)
# define NB_MUTEXES			(5)

# define TINY_BLOCK			( TINY_MAX + sizeof(t_block))
# define SMALL_BLOCK		(SMALL_MAX + sizeof(t_block))
# define LARGE_BLOCK		(SIZE_MAX)

# define TINY_MAX			(256)
# define SMALL_MAX			(2048)
# define LARGE_MAX			(LARGE_BLOCK - sizeof(t_block))

# define TINY_MIN			(1)
# define SMALL_MIN			( TINY_MAX + 1)
# define LARGE_MIN			(SMALL_MAX + 1)

# define TINY_PAGE_SIZE		(8 * getpagesize())
# define SMALL_PAGE_SIZE	(52 * getpagesize())
# define LARGE_PAGE_SIZE	(sizeof(t_seg) + sizeof(t_block))

typedef struct	s_block
{
	size_t			size;

	size_t			prev;
	size_t			next;
}				t_block;

typedef struct	s_seg
{
	size_t			alloc_size;

	size_t			pos;
	size_t			gbl_size;

	struct s_seg	*next;
}				t_seg;

typedef struct	s_env
{
	t_seg			*tiny;
	t_seg			*small;
	t_seg			*large;

	pthread_mutex_t	locks[NB_MUTEXES];
}				t_env;

/*
**	malloc.c
*/
void			*malloc(size_t size);

/*
**	others.c
*/
void			free(void *ptr);
void			*realloc(void *ptr, size_t size);
void			*calloc(size_t nmemb, size_t size);

/*
**	utilities.c
*/
t_env			*mygetenv(void);
void			ft_bzero(void *dst, size_t n);
void			ft_memcpy(void *dst, void *src, size_t size);

/*
**	segments.c
*/
t_seg			*findseg(void *ptr);
void			sortsegs(t_seg **s, t_seg *pa, t_seg *pb);

/*
**	print.c
*/
void			show_alloc_mem(void);

#endif
