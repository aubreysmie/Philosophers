/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 05:48:07 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/21 02:58:47 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sim_thinking(t_philo *philo)
{
	disp_action(philo->number + 1, THINKING, philo->data, NULL);
	if (pthread_create(philo->thread, NULL,
			&perform_death_check_routine, (void *)philo))
	{
		write(2, "An internal error has occured\n", 30);
		sem_close(philo->data->forks);
		exit(ERROR_EXIT_STATUS);
	}
	//perhaps proportionnal waiting function here
	sem_wait(philo->data->forks);
	sem_wait(philo->data->forks);
	pthread_join(philo->thread, NULL);
	disp_action(philo->number + 1, TAKEN_A_FORK,
		philo->data, NULL);
}

void	sim_eating(t_philo *philo)
{
	philo->number_of_times_philo_has_eaten++;
	disp_action(philo->number + 1, EATING, philo->data, NULL);
	gettimeofday(&philo->last_time_philo_ate, NULL);
	if (pthread_create(philo->thread, NULL,
			&perform_death_check_routine, (void *)philo))
	{
		write(2, "An internal error has occured\n", 30);
		sem_close(philo->data->forks);
		exit(ERROR_EXIT_STATUS);
	}
	usleep(philo->data->time_to_eat * 1000);
	pthread_join(philo->thread, NULL);
	if (philo->number_of_times_philo_has_eaten
		== philo->data->number_of_times_each_philo_must_eat)
	{
		sem_close(philo->data->forks);
		exit(DONE_EATING_EXIT_STATUS);
	}
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	sim_sleeping(t_philo *philo)
{
	disp_action(philo->number + 1, SLEEPING, philo->data, NULL);
	if (pthread_create(philo->thread, NULL,
			&perform_death_check_routine, (void *)philo))
	{
		write(2, "An internal error has occured\n", 30);
		sem_close(philo->data->forks);
		exit(ERROR_EXIT_STATUS);
	}
	usleep(philo->data->time_to_sleep * 1000);
	pthread_join(philo->thread, NULL);
}

void	*perform_death_check_routine(void *arg)
{
	t_philo			*philo;
	struct timeval	tv;
	unsigned int	interval;

	philo = (t_philo *)arg;
	gettimeofday(&tv, NULL);
	interval = timeval_to_ms(tv) - timeval_to_ms(philo->last_time_philo_ate);
	if (interval >= philo->data->time_to_die)
	{
		disp_action(philo->number + 1, DIED, philo->data, &tv);
		sem_close(philo->data->forks);
		exit(DEATH_EXIT_STATUS);
	}
	return (NULL);
}

void	sim_philo_routine(t_philo *philo)
{
	usleep(1000);
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