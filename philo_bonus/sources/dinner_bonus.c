/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 05:48:07 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/20 23:14:57 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sim_thinking(t_philo *philo)
{
	disp_action(philo->number + 1, THINKING, philo->data, NULL);
	//perhaps proportionnal waiting function here
	sem_wait(philo->data->forks);
	sem_wait(philo->data->forks);
	disp_action(philo->number + 1, TAKEN_A_FORK,
		philo->data, NULL);
}

void	sim_eating(t_philo *philo)
{
	philo->number_of_times_philo_has_eaten++;
	disp_action(philo->number + 1, EATING, philo->data, NULL);
	//find a way to notice philo ate enough without stopping sim
	usleep(philo->data->time_to_eat * 1000);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	sim_sleeping(t_philo *philo)
{
	disp_action(philo->number + 1, SLEEPING, philo->data, NULL);
	usleep(philo->data->time_to_sleep * 1000);
}

void	sim_philo_routine(t_philo *philo)
{
	usleep(1000);//time for all processes to be created
	if (pthread_create(philo->thread, NULL,
		&death_check_routine, (void *)philo))
	{
		write(2, "An internal error has occured\n", 30);
		destroy_data(philo->data, !UNLINK);
		exit(FUNCTION_ERROR);
	}
	while (true)
	{
		sim_thinking(philo);
		sim_eating(philo);
		sim_sleeping(philo);
	}
}

bool	start_sim(t_data *data)
{
	if (!create_processes(data))
		return (0);
}
