/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 23:26:24 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/10 23:02:00 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	unlink_protection_sems(unsigned int number_of_philos)
{
	unsigned int	i;

	i = 0;
	while (i < number_of_philos)
	{
		sem_unlink((char [3]){'/', i + '0', '\0'});
		i++;
	}
}

void	unlink_sems(unsigned int number_of_philos)
{
	sem_unlink("/print_protection");
	sem_unlink("/forks");
	sem_unlink("/should_stop");
	sem_unlink("/meals");
	unlink_protection_sems(number_of_philos);
}

void	destroy_data(t_data *data, bool should_unlink)
{
	sem_close(data->forks);
	sem_close(data->print_protection);
	sem_close(data->should_stop);
	sem_close(data->meals);
	if (should_unlink)
		unlink_sems(data->number_of_philos);
	free(data->philos);
}
