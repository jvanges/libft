/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-ges <jvan-ges@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 16:22:52 by jvan-ges          #+#    #+#             */
/*   Updated: 2026/10/25 10:24:37 by jvan-ges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	char	*res;
	long	nb;
	size_t	len;

	nb = n;
	len = 1;
	if (nb < 0)
		nb = -nb;
	while (nb >= 10 && ++len)
		nb /= 10;
	if (n < 0)
		len++;
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
