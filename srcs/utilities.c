/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tle-meur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/08 11:49:19 by tle-meur          #+#    #+#             */
/*   Updated: 2017/02/24 17:03:04 by tle-meur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_malloc.h"

t_env		*mygetenv(void)
{
	static t_env	env = { NULL, NULL, NULL, { B, B, B, B, B } };

	return (&env);
}

void		ft_bzero(void *dst, size_t n)
{
	size_t	i;

	if (dst && n && !(i = 0))
		while (i < n)
		{
			((char *)dst)[i] = 0;
			i++;
		}
}

void		ft_memcpy(void *dst, void *src, size_t size)
{
	size_t	i;

	i = 0;
	while (size >= sizeof(uint64_t) && ((size -= sizeof(uint64_t)) || 1))
	{
		*(uint64_t *)(dst + i) = *(uint64_t *)(src + i);
		i += sizeof(uint64_t);
	}
	while (size >= sizeof(uint32_t) && ((size -= sizeof(uint32_t)) || 1))
	{
		*(uint32_t *)(dst + i) = *(uint32_t *)(src + i);
		i += sizeof(uint32_t);
	}
	while (size >= sizeof(uint16_t) && ((size -= sizeof(uint16_t)) || 1))
	{
		*(uint16_t *)(dst + i) = *(uint16_t *)(src + i);
		i += sizeof(uint16_t);
	}
	while (size >= sizeof(uint8_t) && ((size -= sizeof(uint8_t)) || 1))
	{
		*(uint8_t *)(dst + i) = *(uint8_t *)(src + i);
		i += sizeof(uint8_t);
	}
}
