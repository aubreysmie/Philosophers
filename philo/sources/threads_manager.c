/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 13:28:34 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/18 19:21:13 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	join_threads(t_data *data, unsigned int max_thread)
{
	unsigned int	i;

	i = max_thread;
	while (i)
	{
		pthread_join(data->philos[i].thread, NULL);
		i--;
	}
	pthread_join(data->philos[i].thread, NULL);
}

bool	create_threads(t_data *data)
{
	unsigned int	i;

	i = 0;
	pthread_mutex_lock(&data->sim_start_mutex);
	while (i < data->number_of_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				&sim_philo_routine, (void *)(data->philos + i)))
		{
			write(2, "An internal error has occured\n", 30);
			data->sim_status.should_sim_stop = true;
			pthread_mutex_unlock(&data->sim_start_mutex);
			join_threads(data, i);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&data->sim_start_mutex);
	return (1);
}
