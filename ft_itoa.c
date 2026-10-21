/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-ges <jvan-ges@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 16:22:52 by jvan-ges          #+#    #+#             */
/*   Updated: 2026/10/21 16:54:35 by jvan-ges         ###   ########.fr       */
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

	nb = n;
	if (nb < 0)
		nb = -nb;
	len = num_len(nb);
	if (n < 0)
		len++;
	res = malloc(len + 1);
	if (res == NULL)
		return (NULL);
	res[len] = '\0';
	while (len > 0)
	{
		len--;
		res[len] = (nb % 10) + '0';
		nb /= 10;
	}
	if (n < 0)
		res[0] = '-';
	return (res);
}
