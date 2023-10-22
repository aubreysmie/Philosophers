/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 04:21:49 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/23 00:48:28 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	sim_thinking(t_philo *philo)
{
	disp_action(philo->number + 1, THINKING, philo->data);
	pthread_mutex_lock(&philo->left_fork->mutex);
	philo->left_fork->is_used = true;
	disp_action(philo->number + 1, TAKEN_A_FORK, philo->data);
	while (philo->right_fork->is_used)
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		philo->left_fork->is_used = false;
		pthread_mutex_lock(&philo->left_fork->mutex);
		philo->left_fork->is_used = true;
		disp_action(philo->number + 1, TAKEN_A_FORK, philo->data);
	}
	pthread_mutex_lock(&philo->right_fork->mutex);
	philo->left_fork->is_used = true;
	disp_action(philo->number + 1, TAKEN_A_FORK, philo->data);
	return (1);
}

int	sim_eating(t_philo *philo)
{
	disp_action(philo->number + 1, EATING, philo->data);
	gettimeofday(&philo->last_time_philo_ate, NULL);
	usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	philo->left_fork->is_used = false;
	pthread_mutex_unlock(&philo->right_fork->mutex);
	philo->right_fork->is_used = false;
	philo->number_of_times_philo_has_eaten++;
	if (philo->number_of_times_philo_has_eaten
		== philo->data->number_of_times_each_philo_must_eat)
		philo->data->number_of_philos_that_ate_enough++;
	if (philo->data->number_of_philos_that_ate_enough
		== philo->data->number_of_philos)
		return (0);
	return (1);
}

int	sim_sleeping(t_philo *philo)
{
	disp_action(philo->number + 1, SLEEPING, philo->data);
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
	int		i;

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
