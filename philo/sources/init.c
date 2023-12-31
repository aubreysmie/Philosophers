/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 00:39:18 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/11 15:50:42 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	init_forks(t_fork **forks, unsigned int number_of_philos)
{
	unsigned int	i;

	i = 0;
	*forks = malloc(sizeof(t_fork) * number_of_philos);
	if (!*forks)
	{
		write(2, "An internal error has occured\n", 30);
		return (0);
	}
	while (i < number_of_philos)
	{
		pthread_mutex_init(&(*forks)[i].mutex, NULL);
		(*forks)[i].is_taken = false;
		i++;
	}
	return (1);
}

void	init_philo_n(t_philo *philos, t_data *data, unsigned int n)
{
	philos[n].number = n;
	philos[n].left_fork = data->forks + n;
	if (n == (data->number_of_philos - 1))
		philos[n].right_fork = data->forks + 0;
	else
		philos[n].right_fork = data->forks + (n + 1);
	philos[n].last_time_philo_ate = data->ref_time;
	philos[n].number_of_times_philo_has_eaten = 0;
	philos[n].data = data;
}

bool	init_philos(t_philo **philos, t_data *data)
{
	unsigned int	n;

	n = 0;
	*philos = malloc(sizeof(t_philo) * data->number_of_philos);
	if (!*philos)
	{
		write(2, "An internal error has occured\n", 30);
		return (0);
	}
	while (n < data->number_of_philos)
	{
		init_philo_n(*philos, data, n);
		n++;
	}
	return (1);
}

bool	init_data(int argc, char **argv, t_data *data)
{
	data->number_of_philos = ft_atoui(argv[1]);
	data->time_to_die = ft_atoui(argv[2]);
	data->time_to_eat = ft_atoui(argv[3]);
	data->time_to_sleep = ft_atoui(argv[4]);
	if (argc == 6)
		data->number_of_times_each_philo_must_eat = ft_atoui(argv[5]);
	else
		data->number_of_times_each_philo_must_eat = -1;
	if (data->number_of_times_each_philo_must_eat == 0)
		return (0);
	data->number_of_philos_that_ate_enough = 0;
	gettimeofday(&data->ref_time, NULL);
	pthread_mutex_init(&data->sim_status.mutex, NULL);
	data->sim_status.should_sim_stop = false;
	pthread_mutex_init(&data->sim_start_mutex, NULL);
	if (!init_forks(&data->forks, data->number_of_philos))
		return (0);
	if (!init_philos(&data->philos, data))
	{
		destroy_data(data);
		return (0);
	}
	return (1);
}
