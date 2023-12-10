/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:11:42 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/10 23:13:35 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	init_philo_sem(t_semaphore *sem, unsigned int philo_nb)
{
	sem->name = (char [3]){'/', philo_nb + '0', '\0'};
	sem->semaphore
		= sem_open(sem->name, O_CREAT, 666, 1);
	if (sem->semaphore == SEM_FAILED)
	{
		write(2, "An internal error has occured\n", 30);
		return (0);
	}
	return (1);
}

bool	init_data_sem(sem_t **sem, char *sem_name, unsigned int init_val)
{
	*sem = sem_open(sem_name, O_CREAT, 666, init_val);
	if (*sem == SEM_FAILED)
	{
		write(2, "An internal error has occured\n", 30);
		return (0);
	}
	return (1);
}

bool	init_sems(t_data *data)
{
	unlink_sems(data->number_of_philos);
	if (!init_data_sem(&data->print_protection, "/print_protection", 1)
		|| !init_data_sem(&data->forks, "/forks", data->number_of_philos)
		|| !init_data_sem(&data->should_stop, "/should_stop", 0)
		|| !init_data_sem(&data->meals, "/meals", 0))
	{
		sem_close(data->print_protection);
		sem_close(data->forks);
		sem_close(data->should_stop);
		sem_unlink("/print_protection");
		sem_unlink("/forks");
		sem_unlink("/should_stop");
		return (0);
	}
	return (1);
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
		(*philos)[n] = (t_philo){
			.number = n,
			.pid = (pid_t)(-1),
			.thread = (pthread_t)(-1),
			.last_time_philo_ate = data->ref_time,
			.number_of_times_philo_has_eaten = 0,
			.access_protection = (t_semaphore){NULL, NULL},
			.data = data,
		};
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
	if (!init_sems(data))
		return (0);
	gettimeofday(&data->ref_time, NULL);
	if (!init_philos(&data->philos, data))
	{
		destroy_data(data, UNLINK);
		return (0);
	}
	return (1);
}
