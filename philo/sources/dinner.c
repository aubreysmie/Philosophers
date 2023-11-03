/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 04:21:49 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/03 02:12:48 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_to_take_fork(t_philo *philo)
{
	struct timeval			tv;
	// unsigned int			current_time;
	unsigned int			time_since_ate;
	// static pthread_mutex_t	print_lock;

	if (!philo->data->time_to_die || !philo->data->time_to_eat)
		return ;
	// pthread_mutex_lock(&print_lock);
	gettimeofday(&tv, NULL);
	// current_time = timeval_to_ms(tv) - timeval_to_ms(philo->data->ref_time);
	time_since_ate = timeval_to_ms(tv)
		- timeval_to_ms(philo->last_time_philo_ate);
	if (philo->number_of_times_philo_has_eaten == 0)
	{
		// pthread_mutex_unlock(&print_lock);
		return ;
	}
	// dprintf(2, "Time %u : Philo %u will wait time_to_eat\n",
	// 	current_time, philo->number + 1);
	// pthread_mutex_unlock(&print_lock);
	usleep(philo->data->time_to_eat * 1000
		* (1 - ((float) time_since_ate) / (float) philo->data->time_to_die));
}

bool	sim_thinking(t_philo *philo)
{
	disp_action(philo->number + 1, THINKING, philo->data, NULL);
	while (true)
	{
		if (!take_fork(philo->left_fork))
		{
			if (!should_sim_stop(philo))
				break ;
			wait_to_take_fork(philo);
			continue ;
		}
		if (!take_fork(philo->right_fork))
		{
			drop_fork(philo->left_fork);
			// wait_to_take_fork(philo);
			if (!should_sim_stop(philo))
				break ;
			continue ;
		}
		disp_action(philo->number + 1, TAKEN_A_FORK,
			philo->data, NULL);
		return (1);
	}
	return (0);
}

bool	sim_eating(t_philo *philo)
{
	static pthread_mutex_t	fulltum_lock = PTHREAD_MUTEX_INITIALIZER;

	disp_action(philo->number + 1, EATING, philo->data, NULL);
	gettimeofday(&philo->last_time_philo_ate, NULL);
	if (!complete_action(philo, philo->data->time_to_eat))
		return (0);
	drop_fork(philo->left_fork);
	drop_fork(philo->right_fork);
	philo->number_of_times_philo_has_eaten++;
	if (philo->number_of_times_philo_has_eaten
		== philo->data->number_of_times_each_philo_must_eat)
	{
		pthread_mutex_lock(&fulltum_lock);
		philo->data->number_of_philos_that_ate_enough++;
		if (philo->data->number_of_philos_that_ate_enough
			== philo->data->number_of_philos)
		{
			pthread_mutex_lock(&philo->data->sim_status.mutex);
			philo->data->sim_status.should_sim_stop = true;
			pthread_mutex_unlock(&philo->data->sim_status.mutex);
		}
		pthread_mutex_unlock(&fulltum_lock);
	}
	return (1);
}

bool	sim_sleeping(t_philo *philo)
{
	disp_action(philo->number + 1, SLEEPING, philo->data, NULL);
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
	if (philo->number % 2)
		usleep(1000);
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

bool	start_sim(t_data *data)
{
	unsigned int	i;

	pthread_mutex_lock(&data->sim_start_mutex);
	i = -1;
	while (++i < data->number_of_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				&sim_philo_routine, (void *)(data->philos + i)))
		{
			write(2, "An internal error has occured\n", 30);
			return (0);
		}
	}
	pthread_mutex_unlock(&data->sim_start_mutex);
	i = -1;
	while (++i < data->number_of_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL))
		{
			write(2, "An internal error has occured\n", 30);
			return (0);
		}
	}
	return (1);
}
