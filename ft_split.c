/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:09:02 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/12 10:25:32 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

void		*ft_calloc(size_t nmemb, size_t size);
static int	split_str(char **tab, char const *str, char sep, int tab_len);
static int	copy(char **tab, char const *str, int *offset, int *len);
static int	count(char const *str, char sep);
static char	*ft_strndup(char const *src, unsigned int n);

char	**ft_split(char const *s, char c)
{
	char	**tab;
	int		tab_len;
	int		i;

	tab_len = count(s, c) + 1;
	tab = ft_calloc(tab_len + 1, sizeof(char *));
	if (tab == NULL)
		return (NULL);
	if (split_str(tab, s, c, tab_len) == 0)
	{
		i = 0;
		while (i < tab_len)
		{
			if (tab[i] == NULL)
			{
				while (i >= 0)
					free(tab[i--]);
				free(tab);
				return (NULL);
			}
			i++;
		}
	}
	return (tab);
}

static int	split_str(char **tab, char const *str, char sep, int tab_len)
{
	char	*s;
	int		i;
	int		offset;
	int		len;

	s = (char *)str;
	i = 0;
	offset = 0;
	len = 0;
	while (*s && i < tab_len)
	{
		if (*s++ == sep)
		{
			if (len > 0 && !copy(&(tab[i++]), str, &offset, &len))
				return (0);
			offset++;
		}
		else
			len++;
	}
	tab[i] = ft_strndup(str + offset, len);
	return (1);
}

static int	copy(char **tab, char const *str, int *offset, int *len)
{
	char	*r;

	r = ft_strndup(str + *offset, *len);
	if (r == NULL)
		return (0);
	*tab = r;
	*offset += *len;
	*len = 0;
	return (1);
}

static int	count(char const *str, char sep)
{
	int	count;
	int	was_split;

	if (sep == '\0')
		return (0);
	count = 0;
	was_split = 1;
	while (*str)
	{
		if (*str == sep)
		{
			if (was_split == 0)
				count++;
			was_split = 1;
		}
		else
			was_split = 0;
		str++;
	}
	return (count);
}

static char	*ft_strndup(char const *src, unsigned int n)
{
	char			*dest;
	unsigned int	src_len;
	unsigned int	i;

	if (n == 0)
		return (NULL);
	src_len = 0;
	while (src[src_len] && src_len < n)
		src_len++;
	dest = ft_calloc(src_len + 1, sizeof(char));
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (i < src_len)
	{
		dest[i] = src[i];
		i++;
	}
	return (dest);
}
