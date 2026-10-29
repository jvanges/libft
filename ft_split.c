/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvan-ges <jvan-ges@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 16:22:52 by jvan-ges          #+#    #+#             */
/*   Updated: 2026/10/25 10:17:58 by jvan-ges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Counts the words of s, c being the delimiter; runs of c do not create
** empty words, so "  a  b " counts as 2.
*/

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] == '\0')
			break ;
		count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

/*
** Frees the i strings already allocated, then the array itself.
** Used when an allocation fails halfway through ft_split, so nothing
** leaks and NULL can safely be returned.
*/

static void	free_all(char **tab, size_t i)
{
	while (i > 0)
	{
		free(tab[i - 1]);
		i--;
	}
	free(tab);
}

/*
** Advances *pos past the delimiters, then extracts the next word with
** ft_substr, leaving *pos right after it for the following call.
*/

static char	*next_word(char const *s, char c, size_t *pos)
{
	size_t	start;
	size_t	len;

	while (s[*pos] == c)
		(*pos)++;
	start = *pos;
	while (s[*pos] && s[*pos] != c)
		(*pos)++;
	len = *pos - start;
	return (ft_substr(s, start, len));
}

/*
** Splits s into a NULL-terminated array of strings using c as delimiter.
** +1 slot in the malloc makes room for the terminating NULL.
** On any allocation failure, everything allocated so far is freed
** (free_all) and NULL is returned.
*/

char	**ft_split(char const *s, char c)
{
	char	**tab;
	size_t	nb_words;
	size_t	i;
	size_t	pos;

	if (!s)
		return (NULL);
	nb_words = count_words(s, c);
	tab = malloc(sizeof(char *) * (nb_words + 1));
	if (!tab)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < nb_words)
	{
		tab[i] = next_word(s, c, &pos);
		if (!tab[i])
		{
			free_all(tab, i);
			return (NULL);
		}
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
