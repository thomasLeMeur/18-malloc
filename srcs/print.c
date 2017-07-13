/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 16:02:09 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/24 17:00:34 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "ft_malloc.h"

static int			ft_putstr(char *s)
{
	if (s)
		while (*s)
			write(1, s++, 1);
	return (0);
}

static int			ft_putnbr(uintmax_t n)
{
	char	c;

	if (n < 10)
	{
		c = n + '0';
		write(1, &c, 1);
	}
	else
	{
		ft_putnbr(n / 10);
		ft_putnbr(n % 10);
	}
	return (0);
}

static int			ft_puthex(uintmax_t n)
{
	size_t		i;
	size_t		j;
	char		buf[34];
	static char	grammar[17] = "0123456789ABCDEF";

	ft_putstr("0x");
	ft_bzero(buf, (j = 0) + 34 * sizeof(char));
	if (!(i = 0) && n != 0)
	{
		while (n)
		{
			buf[i++] = grammar[n % 16];
			n /= 16;
		}
		while (i < 17)
			buf[i++] = '0';
		i = 0;
		while (i < 8 && ((buf[32] = buf[i]) || 1))
		{
			buf[i] = buf[15 - i];
			buf[15 - i++] = buf[32];
		}
	}
	return (ft_putstr((buf[32] == 0) ? "0000000000000000" : buf));
}

static uintmax_t	show_seg(t_seg *s, char *name, size_t len)
{
	t_block		*b;
	uintmax_t	total;

	total = 0;
	(void)(ft_putstr(name) + ft_puthex((uintmax_t)s) + ft_putstr("\n"));
	while (s != NULL)
	{
		s->pos = sizeof(t_seg);
		while (s->pos < len)
		{
			b = (t_block *)((void *)s + s->pos);
			if (b->size > 0)
			{
				(void)(ft_puthex((uintmax_t)b) + ft_putstr(" - ") +
				ft_puthex((uintmax_t)((void *)b + b->size)) + ft_putstr(" : ")
				+ ft_putnbr(b->size) + ft_putstr(" octets\n"));
			}
			total += b->size;
			s->pos = (s->pos + b->next <= s->pos) ? len : s->pos + b->next;
		}
		s = s->next;
	}
	return (total);
}

void				show_alloc_mem(void)
{
	uintmax_t	total;

	pthread_mutex_lock(&mygetenv()->locks[SHOWMEM_MUTEX]);
	total = 0;
	total += show_seg(mygetenv()->tiny, "TINY  : ", TINY_PAGE_SIZE);
	total += show_seg(mygetenv()->small, "SMALL : ", SMALL_PAGE_SIZE);
	total += show_seg(mygetenv()->large, "LARGE : ", LARGE_PAGE_SIZE);
	(void)(ft_putstr("Total : ") + ft_putnbr(total) + ft_putstr(" octets\n"));
	pthread_mutex_unlock(&mygetenv()->locks[SHOWMEM_MUTEX]);
}
