/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_manager_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/10 22:56:03 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/10 23:14:49 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	end_meals_check(t_data *data)
{
	unsigned int	i;

	i = 0;
	while (i < data->number_of_philos)
	{
		sem_post(data->meals);
		i++;
	}
}

void	create_philo(t_philo *philo)
{
	pthread_t	checker_thread;

	usleep(1000);
	if (!init_philo_sem(&philo->access_protection, philo->number))
	{
		write(2, "An internal error has occured\n", 30);
		sem_post(philo->data->should_stop);
		end_meals_check(philo->data);
		destroy_data(philo->data, !UNLINK);
		exit(EXIT_FAILURE);
	}
	if (pthread_create(&checker_thread, NULL,
			&perform_death_check_routine, (void *)philo))
	{
		write(2, "An internal error has occured\n", 30);
		sem_close(philo->access_protection.semaphore);
		sem_post(philo->data->should_stop);
		end_meals_check(philo->data);
		destroy_data(philo->data, !UNLINK);
		exit(EXIT_FAILURE);
	}
	pthread_detach(checker_thread);
	sim_philo_routine(philo);
}

void	*meals_check_routine(void *arg)
{
	t_data			*data;
	unsigned int	i;

	data = (t_data *)arg;
	if (data->number_of_times_each_philo_must_eat == -1)
		return (NULL);
	i = 0;
	while (i < data->number_of_philos)
	{
		sem_wait(data->meals);
		i++;
	}
	sem_post(data->should_stop);
	return (NULL);
}

bool	stop_sim(t_data *data)
{
	pthread_t		meals_checkers;

	if (pthread_create(&meals_checkers, NULL,
			meals_check_routine, (void *)data))
	{
		write(2, "An internal error has occured\n", 30);
		return (0);
	}
	sem_wait(data->should_stop);
	pthread_join(meals_checkers, NULL);
	destroy_processes(data->philos, data->number_of_philos - 1);
	return (1);
}

bool	start_sim(t_data *data)
{
	if (!create_processes(data))
		return (0);
	if (!stop_sim(data))
		return (0);
	return (1);
}
