/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 05:48:07 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/10 23:08:50 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sim_thinking(t_philo *philo)
{
	disp_action(philo, THINKING, philo->data, NULL);
	wait_for_fork(philo);
	sem_wait(philo->data->forks);
	sem_wait(philo->data->forks);
	disp_action(philo, TAKEN_A_FORK, philo->data, NULL);
}

void	sim_eating(t_philo *philo)
{
	sem_wait(philo->access_protection.semaphore);
	disp_action(philo, EATING, philo->data, NULL);
	gettimeofday(&philo->last_time_philo_ate, NULL);
	sem_post(philo->access_protection.semaphore);
	usleep(philo->data->time_to_eat * 1000);
	philo->number_of_times_philo_has_eaten++;
	if (philo->number_of_times_philo_has_eaten
		== philo->data->number_of_times_each_philo_must_eat)
		sem_post(philo->data->meals);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	sim_sleeping(t_philo *philo)
{
	disp_action(philo, SLEEPING, philo->data, NULL);
	usleep(philo->data->time_to_sleep * 1000);
}

void	sim_philo_routine(t_philo *philo)
{
	while (true)
	{
		sim_thinking(philo);
		sim_eating(philo);
		sim_sleeping(philo);
	}
}
