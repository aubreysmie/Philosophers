/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:11:42 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/02 11:06:58 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	init_protection_sem(t_semaphore *protection_sem, unsigned int philo_nb)
{
	protection_sem->name = (char [3]){'/', philo_nb + '0', '\0'};
	protection_sem->semaphore
		= sem_open(protection_sem->name, O_CREAT, 777, 1);
	if (!protection_sem->semaphore)
	{
		write(2, "An internal error has occured\n", 30);
		return (0);
	}
	return (1);
}

bool	init_forks_sem(sem_t **forks, unsigned int number_of_philos)
{
	*forks = sem_open("/forks", O_CREAT, 777, number_of_philos);
	if (*forks == SEM_FAILED)
	{
		write(2, "An internal error has occured\n", 30);
		return (0);
	}
	return (1);
}

void	init_philo_n(t_philo *philos, t_data *data, unsigned int n)
{
	philos[n].number = n;
	philos[n].pid = (pid_t) -1;
	philos[n].thread = (pthread_t) -1;
	philos[n].last_time_philo_ate = data->ref_time;
	philos[n].number_of_times_philo_has_eaten = 0;
	philos[n].protection_sem = (t_semaphore){NULL, NULL};
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
	data->number_of_philos_that_ate_enough = 0;
	sem_unlink("/forks");
	unlink_protection_sems(data->number_of_philos);
	gettimeofday(&data->ref_time, NULL);
	if (!init_forks_sem(&data->forks, data->number_of_philos))
		return (0);
	if (!init_philos(&data->philos, data))
	{
		sem_close(data->forks);
		sem_unlink("/forks");
		return (0);
	}
	return (1);
}
