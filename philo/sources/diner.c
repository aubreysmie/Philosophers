/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 04:21:49 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/26 13:38:04 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	sim_thinking(t_philo *philo)
{
	while (!philo->data->should_sim_stop)
	disp_action(philo->number + 1, THINKING, philo->data->ref_time, NULL);
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		if (&philo->left_fork->is_taken)
		{
			pthread_mutex_unlock(&philo->left_fork->mutex);
			usleep(10);
			continue ;
		}
		philo->left_fork->is_taken = true;
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_lock(&philo->right_fork->mutex);
		if (&philo->right_fork->is_taken)
		disp_action(philo->number + 1, TAKEN_A_FORK,
			philo->data->ref_time, NULL);
		{
			pthread_mutex_unlock(&philo->right_fork->mutex);
			pthread_mutex_lock(&philo->left_fork->mutex);
			philo->left_fork->is_taken = false;
			pthread_mutex_unlock(&philo->left_fork->mutex);
			usleep(10);
			continue ;
		}
		philo->right_fork->is_taken = true;
		pthread_mutex_unlock(&philo->right_fork->mutex);
		disp_action(philo->number + 1, TAKEN_A_FORK,
			philo->data->ref_time, NULL);
		return (1);
	}
	return (0);
}

bool	sim_eating(t_philo *philo)
{
	static pthread_mutex_t	fulltum_lock = PTHREAD_MUTEX_INITIALIZER;

	disp_action(philo->number + 1, EATING, philo->data->ref_time);
	gettimeofday(&philo->last_time_philo_ate, NULL);
	usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	philo->left_fork->is_taken = false;
	pthread_mutex_unlock(&philo->right_fork->mutex);
	philo->right_fork->is_taken = false;
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
	disp_action(philo->number + 1, SLEEPING, philo->data->ref_time);
	usleep(philo->data->time_to_sleep);
	return (1);
}

void	*sim_philo_routine(void *arg)//write protection for single philo; init mutex with error check
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->data->is_anyone_dead
		&& philo->data->number_of_times_each_philo_must_eat
		!= philo->data->number_of_philos_that_ate_enough)//lacks : deathcheck + fulltum check
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
