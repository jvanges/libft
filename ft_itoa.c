/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-ges <jvan-ges@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 16:22:52 by jvan-ges          #+#    #+#             */
/*   Updated: 2026/10/25 19:52:00 by jvan-ges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Counts the number of decimal digits of nb (taken positive).
** len starts at 1 so that 0 is handled correctly.
*/

static size_t	count_len(long nb)
{
	size_t	len;

	len = 1;
	while (nb >= 10)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

/*
** Converts the integer n to a newly allocated, NUL-terminated string.
** n is handled as a long: -n would overflow for INT_MIN (-2147483648)
** as an int. Digits are written from the end of the string toward the
** beginning, so no reversal is needed; the '-' (if any) lands at index 0.
** Returns NULL if malloc fails.
*/

char	*ft_itoa(int n)
{
	char	*res;
	long	nb;
	size_t	len;

	nb = n;
	if (nb < 0)
		nb = -nb;
	len = count_len(nb) + (n < 0);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	res[len] = '\0';
	while (len > (size_t)(n < 0))
	{
		res[--len] = nb % 10 + '0';
		nb /= 10;
	}
	if (n < 0)
		res[0] = '-';
	return (res);
}
