/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-ges <jvan-ges@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 16:22:52 by jvan-ges          #+#    #+#             */
/*   Updated: 2026/10/21 16:49:38 by jvan-ges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != '\0' && s[i] != c)
			i++;
	}
	return (count);
}

static void	free_all(char **tab, size_t nb_allocated)
{
	while (nb_allocated > 0)
	{
		nb_allocated--;
		free(tab[nb_allocated]);
	}
	free(tab);
}

static char	*next_word(char const *s, char c, size_t *pos)
{
	size_t	start;
	size_t	len;

	while (s[*pos] == c)
		(*pos)++;
	start = *pos;
	while (s[*pos] != '\0' && s[*pos] != c)
		(*pos)++;
	len = *pos - start;
	return (ft_substr(s, start, len));
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	size_t	nb_words;
	size_t	i;
	size_t	pos;

	if (s == NULL)
		return (NULL);
	nb_words = count_words(s, c);
	tab = malloc(sizeof(char *) * (nb_words + 1));
	if (tab == NULL)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < nb_words)
	{
		tab[i] = next_word(s, c, &pos);
		if (tab[i] == NULL)
		{
			free_all(tab, i);
			return (NULL);
		}
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
