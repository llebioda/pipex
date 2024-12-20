/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llebioda <llebioda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 16:43:48 by llebioda          #+#    #+#             */
/*   Updated: 2024/12/11 11:40:42 by llebioda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdint.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	char	*c;
	size_t	s;

	if (nmemb > 0 && SIZE_MAX / nmemb < size)
		return (NULL);
	s = nmemb * size;
	ptr = malloc(s);
	if (ptr == NULL)
		return (NULL);
	c = (char *)ptr;
	while (s > 0)
	{
		*c = 0;
		c++;
		s--;
	}
	return (ptr);
}
