/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 05:48:07 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/09 23:08:03 by ekhaled          ###   ########.fr       */
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
	sem_wait(philo->access_protection.semaphore);
	gettimeofday(&tv, NULL);
	time_since_ate = timeval_to_ms(tv)
		- timeval_to_ms(philo->last_time_philo_ate);
	sem_post(philo->access_protection.semaphore);
	usleep(philo->data->time_to_eat * 1000
		* (1 - ((float) time_since_ate) / (float) philo->data->time_to_die));
}

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

void	sim_philo_routine(t_philo *philo)
{
	while (true)
	{
		sim_thinking(philo);
		sim_eating(philo);
		sim_sleeping(philo);
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
		exit(ERROR_EXIT_STATUS);
	}
	if (pthread_create(&checker_thread, NULL,
			&perform_death_check_routine, (void *)philo))
	{
		write(2, "An internal error has occured\n", 30);
		sem_close(philo->access_protection.semaphore);
		sem_post(philo->data->should_stop);
		end_meals_check(philo->data);
		destroy_data(philo->data, !UNLINK);
		exit(ERROR_EXIT_STATUS);
	}
	pthread_detach(checker_thread);
	while (true)
	{
		sim_thinking(philo);
		sim_eating(philo);
		sim_sleeping(philo);
	}
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

	if (pthread_create(&meals_checkers, NULL, meals_check_routine, (void *)data))
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
