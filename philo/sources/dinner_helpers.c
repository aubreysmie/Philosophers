/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 12:56:38 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/29 15:10:53 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	timeval_to_ms(struct timeval tv)
{
	return (tv.tv_usec / 1000 + tv.tv_sec * 1000);
}

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
	struct timeval			tv;
	unsigned int		interval;

	gettimeofday(&tv, NULL);
	interval = timeval_to_ms(tv) - timeval_to_ms(philo->last_time_philo_ate);
	// dprintf(2, "Interval : %lu\n", interval);
	if (interval > philo->data->time_to_die)
	{
		disp_action(philo->number + 1, DIED, philo->data, &tv);
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
	unsigned int	interval;

	gettimeofday(&tv, NULL);
	interval = timeval_to_ms(tv) - timeval_to_ms(philo->last_time_philo_ate);
	if (interval + time_for_action < philo->data->time_to_die)
	{
		usleep(time_for_action * 1000);
		return (should_sim_stop(philo));
	}
	// dprintf(2, "%sNot enough time before death to carry out action%s\n", KRED, KEND);
	usleep((philo->data->time_to_die - interval) * 1000);
	return (should_sim_stop(philo));
}
