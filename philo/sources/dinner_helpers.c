/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 12:56:38 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/26 13:35:20 by ekhaled          ###   ########.fr       */
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

bool	should_sim_stop(t_philo *philo)
{
	struct timeval	tv;
	long int		interval;

	gettimeofday(&tv, NULL);
	interval = (tv.tv_usec - philo->last_time_philo_ate.tv_usec)
		+ (tv.tv_sec - philo->last_time_philo_ate.tv_sec) * 1000000;
	if (interval > philo->data->time_to_die)
	{
		disp_action(philo->number + 1, DIED, philo->data->ref_time, &tv);
		pthread_mutex_lock(&philo->data->sim_status.mutex);
		philo->data->sim_status.should_sim_stop = true;
		pthread_mutex_unlock(&philo->data->sim_status.mutex);
		return (0);
	}
	pthread_mutex_lock(&philo->data->sim_status.mutex);
	if (philo->data->sim_status.should_sim_stop)
	{
		pthread_mutex_unlock(&philo->data->sim_status.mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->sim_status.mutex);
	return (1);
}

bool	complete_action(t_philo *philo, unsigned int time_for_action)
{
	struct timeval		tv;
	unsigned int		interval;

	gettimeofday(&tv, NULL);
	interval = (tv.tv_usec - philo->last_time_philo_ate.tv_usec)
		+ (tv.tv_sec - philo->last_time_philo_ate.tv_sec) * 1000000;
	if (interval + time_for_action < philo->data->time_to_die)
	{
		usleep(time_for_action);
		return (should_sim_stop(philo));
	}
	usleep(philo->data->time_to_die - interval);
	return (should_sim_stop(philo));
}
