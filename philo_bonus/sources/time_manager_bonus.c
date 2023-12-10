/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_manager_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 21:57:41 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/10 23:02:32 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

unsigned int	timeval_to_ms(struct timeval tv)
{
	return (tv.tv_usec / 1000 + tv.tv_sec * 1000);
}

void	*perform_death_check_routine(void *arg)
{
	t_philo			*philo;
	struct timeval	tv;
	unsigned int	interval;

	philo = (t_philo *)arg;
	while (true)
	{
		sem_wait(philo->access_protection.semaphore);
		gettimeofday(&tv, NULL);
		interval = timeval_to_ms(tv)
			- timeval_to_ms(philo->last_time_philo_ate);
		sem_post(philo->access_protection.semaphore);
		if (interval >= philo->data->time_to_die)
		{
			disp_action(philo, DIED, philo->data, &tv);
			sem_post(philo->data->should_stop);
			end_meals_check(philo->data);
			return (NULL);
		}
		usleep(3000);
	}
}

void	wait_for_fork(t_philo *philo)
{
	struct timeval			tv;
	unsigned int			time_since_ate;

	if (!philo->data->time_to_die)
		return ;
	if (philo->number_of_times_philo_has_eaten == 0)
		return ;
	sem_wait(philo->access_protection.semaphore);
	gettimeofday(&tv, NULL);
	time_since_ate = timeval_to_ms(tv)
		- timeval_to_ms(philo->last_time_philo_ate);
	sem_post(philo->access_protection.semaphore);
	usleep(philo->data->time_to_eat * 1000
		* (1 - ((float) time_since_ate) / (float) philo->data->time_to_die));
}
