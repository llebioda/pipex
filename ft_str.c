/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 09:30:22 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/12 10:24:03 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2, char const *s3)
{
	char	*dst;
	size_t	len_s1;
	size_t	len_s2;
	size_t	len_s3;
	size_t	i;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	len_s3 = ft_strlen(s3);
	dst = malloc(len_s1 + len_s2 + len_s3 + 1);
	if (dst == NULL)
		return (NULL);
	i = 0;
	while (i < len_s1 || i < len_s2 || i < len_s3)
	{
		if (i < len_s1)
			dst[i] = s1[i];
		if (i < len_s2)
			dst[i + len_s1] = s2[i];
		if (i < len_s3)
			dst[i + len_s1 + len_s2] = s3[i];
		i++;
	}
	dst[len_s1 + len_s2 + len_s3] = '\0';
	return (dst);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (s1 == NULL || s2 == NULL)
		return (s1 != s2);
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (s1 == NULL || s2 == NULL)
		return (s1 != s2);
	if (n == 0)
		return (0);
	while (*s1 && *s1 == *s2 && n > 1)
	{
		s1++;
		s2++;
		n--;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
