/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 04:21:49 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/18 14:15:46 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->data->is_anyone_dead)
	{
		if (philo->has_just_slept)
			//disp is thinking
		philo->has_just_slept = false;
		pthread_mutex_lock(&philo->left_fork.mutex);
		philo->left_fork.is_used = true;
		//disp took fork
		if (philo->right_fork.is_used)
		{
			pthread_mutex_unlock(&philo->left_fork.mutex);
			continue;
		}
		pthread_mutex_lock(&philo->right_fork.mutex);
		//disp is eating
		//usleep for is eating
		pthread_mutex_unlock(&philo->left_fork.mutex);
		pthread_mutex_unlock(&philo->right_fork.mutex);
		philo->number_of_times_philosopher_has_eaten++;
		if (philo->number_of_times_philosopher_has_eaten
			== philo->data->number_of_times_each_philosopher_must_eat)
			break;
		//disp is sleeping
		//usleep for is sleeping
		philo->has_just_slept = true;
	}
}

bool	start_sim(t_data *data)
{
	int		i;
	void	*retval;

	i = -1;
	while (++i < data->number_of_philosophers)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
			&start_philo_routine, (void *)data->philo[i]))//the thread needs to know which philo it is
		{
			write(2, "An internal error has occured\n", 30);
			return (0);
		}
	}
	i = -1;
	while (++i < data->number_of_philosophers)
	{
		if (pthread_join(data->philos[i].thread, &retval)
		|| retval == NULL)
		{
			write(2, "An internal error has occured\n", 30);
			return (0);
		}
	}
	return (1);
}
