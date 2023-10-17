/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 00:39:18 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/18 00:56:01 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_philo_n(t_data *data, unsigned int n)
{
	data->philos[n].number = n;
}

bool	init_philos(t_data *data)
{
	unsigned int	n;

	n = 0;
	data->philos = malloc(sizeof(t_philo) * data->number_of_philosophers);
	if (!data->philos)
		return (0);
	while (n < data->number_of_philosophers)
	{
		init_philo_n(data, n);
		n++;
	}
	return (1);
}

bool	init_data(int argc, char **argv, t_data *data)
{
	data->number_of_philosophers = ft_atoui(argv[1]);
	data->time_to_die = ft_atoui(argv[2]);
	data->time_to_eat = ft_atoui(argv[3]);
	data->time_to_sleep = ft_atoui(argv[4]);
	if (argc == 6)
		data->number_of_times_each_philosopher_must_eat = ft_atoui(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = -1;
	if (!init_philos(data))
		return (0);
	return (1);
}
