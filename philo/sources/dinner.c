/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 04:21:49 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/18 18:45:09 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

bool	sim_thinking(t_philo *philo)
{
	if (!should_sim_stop(philo))
		return (0);
	safe_disp_action(philo->number + 1, THINKING, philo->data, NULL);
	wait_for_fork(philo);
	while (true)
	{
		if (!take_fork(philo->left_fork))
		{
			if (!should_sim_stop(philo))
				break ;
			wait_for_fork(philo);
			continue ;
		}
		if (!take_fork(philo->right_fork))
		{
			drop_fork(philo->left_fork);
			if (!should_sim_stop(philo))
				break ;
			continue ;
		}
		safe_disp_action(philo->number + 1, TAKEN_A_FORK,
			philo->data, NULL);
		return (1);
	}
	return (0);
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

bool	sim_eating(t_philo *philo)
{
	if (!should_sim_stop(philo))
		return (0);
	philo->number_of_times_philo_has_eaten++;
	if (!is_enough_meals(philo))
		safe_disp_action(philo->number + 1, EATING, philo->data, NULL);
	gettimeofday(&philo->last_time_philo_ate, NULL);
	if (!complete_action(philo, philo->data->time_to_eat))
		return (0);
	drop_fork(philo->left_fork);
	drop_fork(philo->right_fork);
	return (1);
}

bool	sim_sleeping(t_philo *philo)
{
	if (!should_sim_stop(philo))
		return (0);
	safe_disp_action(philo->number + 1, SLEEPING, philo->data, NULL);
	if (!complete_action(philo, philo->data->time_to_sleep))
		return (0);
	return (1);
}

void	*sim_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->sim_start_mutex);
	pthread_mutex_unlock(&philo->data->sim_start_mutex);
	pthread_mutex_lock(&philo->data->sim_status.mutex);
	if (philo->data->sim_status.should_sim_stop)
	{
		pthread_mutex_unlock(&philo->data->sim_status.mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&philo->data->sim_status.mutex);
	if (philo->number % 2)
		usleep(2000);
	while (true)
	{
		if (!sim_thinking(philo))
			break ;
		if (!sim_eating(philo))
			break ;
		if (!sim_sleeping(philo))
			break ;
	}
	return (NULL);
}

