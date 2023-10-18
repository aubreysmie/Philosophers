/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:19:55 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/18 18:48:01 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(t_fork *forks, unsigned int number_of_philos)
{
	int	i;

	i = -1;
	while (++i < number_of_philos)
		pthread_mutex_destroy(&forks[i].mutex);
	free(forks);
}

