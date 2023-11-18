/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 12:56:38 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/18 19:16:35 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	take_fork(t_fork *fork)
{
	pthread_mutex_lock(&fork->mutex);
	if (fork->is_taken)
	{
		pthread_mutex_unlock(&fork->mutex);
		return (0);
	}
	fork->is_taken = true;
	pthread_mutex_unlock(&fork->mutex);
	return (1);
}

void	drop_fork(t_fork *fork)
{
	pthread_mutex_lock(&fork->mutex);
	fork->is_taken = false;
	pthread_mutex_unlock(&fork->mutex);
}

void	wait_for_fork(t_philo *philo)
{
	struct timeval			tv;
	unsigned int			time_since_ate;

	if (!philo->data->time_to_die)
		return ;
	gettimeofday(&tv, NULL);
	time_since_ate = timeval_to_ms(tv)
		- timeval_to_ms(philo->last_time_philo_ate);
	if (philo->number_of_times_philo_has_eaten == 0)
		return ;
	usleep(philo->data->time_to_eat * 1000
		* (1 - ((float) time_since_ate) / (float) philo->data->time_to_die));
}

bool	is_enough_meals(t_philo *philo)
{
	static pthread_mutex_t	full_philo_lock = PTHREAD_MUTEX_INITIALIZER;

	if (philo->number_of_times_philo_has_eaten
		== philo->data->number_of_times_each_philo_must_eat)
	{
		pthread_mutex_lock(&full_philo_lock);
		philo->data->number_of_philos_that_ate_enough++;
		if (philo->data->number_of_philos_that_ate_enough
			== philo->data->number_of_philos)
		{
			pthread_mutex_unlock(&full_philo_lock);
			pthread_mutex_lock(&philo->data->sim_status.mutex);
			philo->data->sim_status.should_sim_stop = true;
			disp_action(philo->number + 1, EATING, philo->data, NULL);
			pthread_mutex_unlock(&philo->data->sim_status.mutex);
			return (1);
		}
		pthread_mutex_unlock(&full_philo_lock);
	}
	return (0);
}
