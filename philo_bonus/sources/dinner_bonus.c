/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 05:48:07 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/09 12:34:52 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	wait_for_fork(t_philo *philo)
{
	struct timeval			tv;
	unsigned int			time_since_ate;

	if (!philo->data->time_to_die)
		return ;
	if (philo->number_of_times_philo_has_eaten == 0)
		return ;
	sem_wait(philo->access_protection_sem.semaphore);
	gettimeofday(&tv, NULL);
	time_since_ate = timeval_to_ms(tv)
		- timeval_to_ms(philo->last_time_philo_ate);
	sem_post(philo->access_protection_sem.semaphore);
	usleep(philo->data->time_to_eat * 1000
		* (1 - ((float) time_since_ate) / (float) philo->data->time_to_die));
}

bool	sim_thinking(t_philo *philo)
{
	disp_action(philo, THINKING, philo->data, NULL);
	wait_for_fork(philo);
	sem_wait(philo->philo_status.status_protection_sem.semaphore);
	if (philo->philo_status.should_philo_stop)
		return (0);
	sem_post(philo->philo_status.status_protection_sem.semaphore);
	sem_wait(philo->data->forks);
	sem_wait(philo->data->forks);
	sem_wait(philo->philo_status.status_protection_sem.semaphore);
	if (philo->philo_status.should_philo_stop)
		return (0);
	sem_post(philo->philo_status.status_protection_sem.semaphore);
	disp_action(philo, TAKEN_A_FORK,
		philo->data, NULL);
	return (1);
}

bool	sim_eating(t_philo *philo)
{
	sem_wait(philo->philo_status.status_protection_sem.semaphore);
	if (philo->philo_status.should_philo_stop)
		return (0);
	sem_post(philo->philo_status.status_protection_sem.semaphore);
	sem_wait(philo->access_protection_sem.semaphore);
	philo->number_of_times_philo_has_eaten++;
	disp_action(philo, EATING, philo->data, NULL);
	gettimeofday(&philo->last_time_philo_ate, NULL);
	sem_post(philo->access_protection_sem.semaphore);
	usleep(philo->data->time_to_eat * 1000);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
	sem_wait(philo->philo_status.status_protection_sem.semaphore);
	if (philo->philo_status.should_philo_stop)
		return (0);
	sem_post(philo->philo_status.status_protection_sem.semaphore);
	return (1);
}

bool	sim_sleeping(t_philo *philo)
{
	sem_wait(philo->philo_status.status_protection_sem.semaphore);
	if (philo->philo_status.should_philo_stop)
		return (0);
	sem_post(philo->philo_status.status_protection_sem.semaphore);
	disp_action(philo, SLEEPING, philo->data, NULL);
	usleep(philo->data->time_to_sleep * 1000);
	return (1);
}

void	*perform_death_check_routine(void *arg)
{
	t_philo			*philo;
	struct timeval	tv;
	unsigned int	interval;

	philo = (t_philo *)arg;
	while (true)
	{
		sem_wait(philo->access_protection_sem.semaphore);
		gettimeofday(&tv, NULL);
		interval = timeval_to_ms(tv)
			- timeval_to_ms(philo->last_time_philo_ate);
		sem_post(philo->access_protection_sem.semaphore);
		if (interval >= philo->data->time_to_die)
		{
			sem_wait(philo->data->death_print_protection_sem);
			disp_action(philo, DIED, philo->data, &tv);
			sem_wait(philo->philo_status.status_protection_sem.semaphore);
			philo->philo_status.should_philo_stop = true;
			sem_post(philo->philo_status.status_protection_sem.semaphore);
			return ((void *)DEATH_EXIT_STATUS);
		}
		sem_wait(philo->access_protection_sem.semaphore);
		if (philo->number_of_times_philo_has_eaten
			== philo->data->number_of_times_each_philo_must_eat)
		{
			sem_post(philo->access_protection_sem.semaphore);
			sem_wait(philo->philo_status.status_protection_sem.semaphore);
			philo->philo_status.should_philo_stop = true;
			sem_post(philo->philo_status.status_protection_sem.semaphore);
			return ((void *)DONE_EATING_EXIT_STATUS);
		}
		sem_post(philo->access_protection_sem.semaphore);
		usleep(5000);
	}
	return (NULL);
}

void	*sim_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (true)
	{
		if (!sim_thinking(philo))
			return (NULL);
		if (!sim_eating(philo))
			return (NULL);
		if (!sim_sleeping(philo))
			return (NULL);
	}
}

void	create_philo(t_philo *philo)
{
	pthread_t	checker_thread;
	pthread_t	routine_thread;
	void		*retval;

	usleep(1000);
	if (!init_philo_sem(&philo->access_protection_sem, philo->number, ACCESS_SEM)
		|| !init_philo_sem(&philo->print_protection_sem, philo->number, PRINT_SEM)
		|| !init_philo_sem(&philo->philo_status.status_protection_sem, philo->number, STATUS_SEM))
	{
		sem_close(philo->philo_status.status_protection_sem.semaphore);
		sem_close(philo->access_protection_sem.semaphore);
		sem_close(philo->print_protection_sem.semaphore);
		destroy_data(philo->data, !UNLINK);
		exit(ERROR_EXIT_STATUS);
	}
	if (pthread_create(&checker_thread, NULL,
			&perform_death_check_routine, (void *)philo))
	{
		sem_close(philo->access_protection_sem.semaphore);
		sem_close(philo->print_protection_sem.semaphore);
		destroy_data(philo->data, !UNLINK);
		exit(ERROR_EXIT_STATUS);
	}
	if (pthread_create(&routine_thread, NULL,
			&sim_philo_routine, (void *)philo))
	{
		sem_close(philo->access_protection_sem.semaphore);
		sem_close(philo->print_protection_sem.semaphore);
		destroy_data(philo->data, !UNLINK);
		exit(ERROR_EXIT_STATUS);
	}
	pthread_join(checker_thread, &retval);
	pthread_join(routine_thread, NULL);
	sem_close(philo->philo_status.status_protection_sem.semaphore);
	sem_close(philo->access_protection_sem.semaphore);
	sem_close(philo->print_protection_sem.semaphore);
	destroy_data(philo->data, !UNLINK);
	exit((long)retval);
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
	if (!create_threads(data, checking_threads))
	{
		free(checking_threads);
		return (0);
	}
	if (!join_threads(checking_threads,
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
	return (1);
}
