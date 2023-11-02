/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 04:21:49 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/02 19:07:14 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_to_take_fork(t_philo *philo)
{
	struct timeval			tv;
	unsigned int			current_time;
	unsigned int			time_since_ate;
	static pthread_mutex_t	print_lock;

	if (!philo->data->time_to_die)
		return ;
	pthread_mutex_lock(&print_lock);
	gettimeofday(&tv, NULL);
	// dprintf(2, "Hi do we ger here\n");
	time_since_ate = timeval_to_ms(tv) - timeval_to_ms(philo->last_time_philo_ate);
	current_time = timeval_to_ms(tv) - timeval_to_ms(philo->data->ref_time);
	// dprintf(2, "%sPhilo %d : Time : %u, time since ate : %u, time to die : %u%s\n", KGRN,
	// 	philo->number + 1, current_time, time_since_ate, philo->data->time_to_die, KEND);
	// dprintf(2, "time to die * 2/3 : %f\n", philo->data->time_to_die * ((float) 2) / ((float) 3));
	if (time_since_ate > philo->data->time_to_die * ((float) 9) / ((float) 10))
	{
		dprintf(2, "%u Philo %u got priviledged (ate %ums ago)\n",
			current_time, philo->number + 1, time_since_ate);
		pthread_mutex_unlock(&print_lock);
		return ;
	}
	pthread_mutex_unlock(&print_lock);
	// dprintf(2, "%sPhilo %u has to wait%s\n", KCYN, philo->number + 1, KEND);
	usleep(3000 * ((float) time_since_ate / (float) philo->data->time_to_die));
	// dprintf(2, "%sPhilo %u finished waiting%s\n", KMAG, philo->number + 1, KEND);
	// return ;
	// usleep(1000 + 5000 * (1 - time_since_ate / philo->data->time_to_die));
}

bool	sim_thinking(t_philo *philo)
{
	disp_action(philo->number + 1, THINKING, philo->data, NULL);
	while (true)
	{
		if (!take_fork(philo->left_fork))
		{
			// usleep(1000);
			wait_to_take_fork(philo);
			if (!should_sim_stop(philo))
			{
				// dprintf(2, "%sBroke on first fork try%s\n", KYEL, KEND);
				break ;
			}
			continue ;
		}
		// disp_action(philo->number + 1, TAKEN_A_FORK,
		// 	philo->data, NULL);
		if (!take_fork(philo->right_fork))
		{
			drop_fork(philo->left_fork);
			// usleep(1000);
			wait_to_take_fork(philo);
			if (!should_sim_stop(philo))
			{
				// dprintf(2, "%sBroke on second fork try%s\n", KYEL, KEND);
				break ;
			}
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
	// dprintf(2, "%sNumber of time philo %u has eaten : %u%s\n",
		// KCYN, philo->number+ 1, philo->number_of_times_philo_has_eaten, KEND);
	// dprintf(2, "%sNumber of time each philo must eat : %u%s\n", KGRN,
		// philo->data->number_of_times_each_philo_must_eat, KEND);
	if (philo->number_of_times_philo_has_eaten
		== philo->data->number_of_times_each_philo_must_eat)
	{
		// dprintf(2, "%sDo we ever get here%s\n", KMAG, KEND);
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
	// dprintf(2, "hi\n");
	pthread_mutex_lock(&philo->data->sim_start_mutex);
	pthread_mutex_unlock(&philo->data->sim_start_mutex);
	// usleep(100);
	while (true)
	{
		if (!sim_thinking(philo))
		{
			// dprintf(2, "%sBroke on think%s\n", KRED, KEND);
			break ;
		}
		if (!sim_eating(philo))
		{
			// dprintf(2, "%sBroke on eat%s\n", KRED, KEND);
			break ;
		}
		if (!sim_sleeping(philo))
		{
			// dprintf(2, "%sBroke on sleep%s\n", KRED, KEND);
			break ;
		}
		// dprintf(2, "Sleep over\n");
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
