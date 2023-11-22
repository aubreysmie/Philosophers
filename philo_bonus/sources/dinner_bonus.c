/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 05:48:07 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/22 10:12:17 by ekhaled          ###   ########.fr       */
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
	sem_wait(philo->protection_sem.semaphore);
	disp_action(philo->number + 1, EATING, philo->data, NULL);
	gettimeofday(&philo->last_time_philo_ate, NULL);
	sem_post(philo->protection_sem.semaphore);
	usleep(philo->data->time_to_eat * 1000);
	if (philo->number_of_times_philo_has_eaten
		== philo->data->number_of_times_each_philo_must_eat)
	{
		// destroy_data(philo->data, !UNLINK);
		exit(DONE_EATING_EXIT_STATUS);
	}
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	sim_sleeping(t_philo *philo)
{
	disp_action(philo->number + 1, SLEEPING, philo->data, NULL);
	usleep(philo->data->time_to_sleep * 1000);
}

void	*perform_death_check_routine(void *arg)
{
	t_philo			*philo;
	struct timeval	tv;
	unsigned int	interval;

	philo = (t_philo *)arg;
	while (true)
	{
		sem_wait(philo->protection_sem.semaphore);
		gettimeofday(&tv, NULL);
		interval = timeval_to_ms(tv)
			- timeval_to_ms(philo->last_time_philo_ate);
		sem_post(philo->protection_sem.semaphore);
		if (interval >= philo->data->time_to_die)
		{
			disp_action(philo->number + 1, DIED, philo->data, &tv);
			sem_close(philo->data->forks);
			sem_close(philo->protection_sem.semaphore);
			sem_unlink(philo->protection_sem.name);
			exit(DEATH_EXIT_STATUS);
		}
		usleep(5000);
	}
	return (NULL);
}

void	sim_philo_routine(t_philo *philo)
{
	usleep(1000);
	if (!init_protection_sem(&philo->protection_sem, philo->number))
	{
		sem_close(philo->data->forks);
		exit(ERROR_EXIT_STATUS);
	}
	if (pthread_create(philo->thread, NULL,
			&perform_death_check_routine, (void *)philo))
	{
		write(2, "An internal error has occured\n", 30);
		sem_close(philo->data->forks);
		sem_close(philo->protection_sem.semaphore);
		sem_unlink(philo->protection_sem.name);
		exit(ERROR_EXIT_STATUS);
	}
	while (true)
	{
		sim_thinking(philo);
		sim_eating(philo);
		sim_sleeping(philo);
	}
}

bool	stop_sim(t_data *data)
{
	pthread_t		*checking_threads;

	checking_threads = malloc(sizeof(pthread_t) * data->number_of_philos);
	if (!checking_threads)
	{
		write(2, "An internal error has occured\n", 30);
		return (0);
	}
	if (!create_threads(data, checking_threads)
		|| !join_threads(checking_threads,
			data->number_of_philos - 1, RETURN_ERROR))
	{
		free(checking_threads);
		return (0);
	}
	free(checking_threads);
	return (1);
}

bool	start_sim(t_data *data)
{
	if (!create_processes(data))
		return (0);
	if (!stop_sim(data))
		return (0);
}
