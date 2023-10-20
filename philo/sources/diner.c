/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 04:21:49 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/20 19:25:47 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->data->is_anyone_dead
		&& philo->data->number_of_times_each_philo_must_eat
		!= philo->data->how_many_philos_ate_enough)//lacks : deathcheck + fulltum check
	{
		if (philo->has_just_slept)//possible de le mettre dans le disp_action ?
			disp_action(philo->number + 1, THINKING, philo->data);
		philo->has_just_slept = false;
		pthread_mutex_lock(&philo->left_fork->mutex);
		philo->left_fork->is_used = true;
		disp_action(philo->number + 1, TAKEN_A_FORK, philo->data);
		if (philo->right_fork->is_used)
		{
			pthread_mutex_unlock(&philo->left_fork->mutex);
			continue;
		}
		pthread_mutex_lock(&philo->right_fork->mutex);
		disp_action(philo->number + 1, EATING, philo->data);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
		philo->number_of_times_philo_has_eaten++;
		if (philo->number_of_times_philo_has_eaten
			== philo->data->number_of_times_each_philo_must_eat)
			philo->data->how_many_philos_ate_enough++;
		disp_action(philo->number + 1, SLEEPING, philo->data);
		philo->has_just_slept = true;// possible de le mettre dans le disp_action ?
	}
}

bool	start_sim(t_data *data)
{
	int		i;

	i = -1;
	while (++i < data->number_of_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
			&start_philo_routine, (void *)data->philos[i]))
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
