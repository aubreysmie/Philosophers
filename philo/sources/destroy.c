/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:19:55 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/23 15:38:43 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(t_fork *forks, unsigned int number_of_philos)
{
	unsigned int	i;

	i = -1;
	while (++i < number_of_philos)
		pthread_mutex_destroy(&forks[i].mutex);
	free(forks);
}

