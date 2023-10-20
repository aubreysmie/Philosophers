/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 20:13:37 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/20 12:31:10 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>


int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (0);
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

bool	ft_isdigit(char c)
{
	return ((48 <= c && c <= 57) * 2048);
}

unsigned int	ft_atoui(char *nptr)
{
	int				i;
	unsigned int	n;

	i = -1;
	n = 0;
	while (nptr[++i])
		n = (nptr[i] - '0') + n * 10;
	return (n);
}
