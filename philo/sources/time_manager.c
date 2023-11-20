/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_manager.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 19:14:44 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/20 22:10:02 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	timeval_to_ms(struct timeval tv)
{
	return (tv.tv_usec / 1000 + tv.tv_sec * 1000);
}

bool	should_sim_stop(t_philo *philo)
{
	struct timeval			tv;
	unsigned int			interval;

	gettimeofday(&tv, NULL);
	interval = timeval_to_ms(tv) - timeval_to_ms(philo->last_time_philo_ate);
	pthread_mutex_lock(&philo->data->sim_status.mutex);
	if (philo->data->sim_status.should_sim_stop)
	{
		pthread_mutex_unlock(&philo->data->sim_status.mutex);
		return (0);
	}
	if (interval > philo->data->time_to_die)
	{
		philo->data->sim_status.should_sim_stop = true;
		disp_action(philo->number + 1, DIED, philo->data, &tv);
		pthread_mutex_unlock(&philo->data->sim_status.mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->sim_status.mutex);
	return (1);
}

bool	complete_action(t_philo *philo, unsigned int time_for_action)
{
	struct timeval		tv;
	unsigned int		time_since_ate;

	gettimeofday(&tv, NULL);
	time_since_ate = timeval_to_ms(tv)
		- timeval_to_ms(philo->last_time_philo_ate);
	if (time_since_ate + time_for_action < philo->data->time_to_die)
	{
		usleep(time_for_action * 1000);
		return (should_sim_stop(philo));
	}
	usleep((philo->data->time_to_die - time_since_ate) * 1000);
	pthread_mutex_lock(&philo->data->sim_status.mutex);
	gettimeofday(&tv, NULL);
	if (!philo->data->sim_status.should_sim_stop)
	{
		philo->data->sim_status.should_sim_stop = true;
		disp_action(philo->number + 1, DIED, philo->data, &tv);
	}
	pthread_mutex_unlock(&philo->data->sim_status.mutex);
	return (0);
}
