/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-ges <jvan-ges@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 16:22:52 by jvan-ges          #+#    #+#             */
/*   Updated: 2026/10/15 16:22:52 by jvan-ges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	num_len(long n)
{
	size_t	len;

	len = 1;
	while (n >= 10)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*res;
	long	nb;
	size_t	len;
	size_t	neg;

	nb = n;
	neg = (n < 0);
	if (neg)
		nb = -nb;
	len = num_len(nb) + neg;
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	if (neg)
		res[0] = '-';
	while (len > neg)
	{
		len--;
		res[len] = nb % 10 + '0';
		nb /= 10;
	}
	return (res);
}
