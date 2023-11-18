/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:19:55 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/18 23:31:43 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_forks(t_fork *forks, unsigned int number_of_philos)
{
	unsigned int	i;

	i = 0;
	while (i < number_of_philos)
	{
		pthread_mutex_destroy(&forks[i].mutex);
		i++;
	}
	free(forks);
}

void	destroy_data(t_data *data)
{
	pthread_mutex_destroy(&data->sim_status.mutex);
	pthread_mutex_destroy(&data->sim_start_mutex);
	destroy_forks(data->forks, data->number_of_philos);
	free(data->philos);
}
