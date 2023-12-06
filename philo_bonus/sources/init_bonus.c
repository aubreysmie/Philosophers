/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 00:11:42 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/06 14:09:16 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	init_philo_sem(t_semaphore *sem, unsigned int philo_nb,
	enum e_protection_sem_type sem_type)
{
	char	c;

	if (sem_type == ACCESS_SEM)
		c = 'a';
	if (sem_type == PRINT_SEM)
		c = 'p';
	sem->name = (char [4]){'/', c, philo_nb + '0', '\0'};
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
	// sem_unlink("/forks");
	*sem = sem_open(sem_name, O_CREAT, 666, init_val);
	if (*sem == SEM_FAILED)
	{
		// dprintf(2, "%sinit_print_protection_sem error%s\n",
			// KGRN, KEND);
		write(2, "An internal error has occured\n", 30);
		return (0);
	}
	return (1);
}

bool	init_sems(t_data *data)
{
	unlink_sems(data->number_of_philos);
	if (!init_data_sem(&data->death_print_protection_sem,
			"/death_print_protection_sem", 1))
		return (0);
	if (!init_data_sem(&data->forks, "/forks", data->number_of_philos))
	{
		sem_close(data->death_print_protection_sem);
		sem_unlink("/death_print_protection_sem");
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
	philos[n].access_protection_sem = (t_semaphore){NULL, NULL};
	philos[n].print_protection_sem = (t_semaphore){NULL, NULL};
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
