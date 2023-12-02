/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 23:26:24 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/02 12:06:38 by ekhaled          ###   ########.fr       */
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

void	destroy_data(t_data *data, bool should_unlink)
{
	sem_close(data->forks);
	sem_close(data->print_protection_sem);
	if (should_unlink)
	{
		sem_unlink("/forks");
		sem_unlink("/print_protection_sem");
	}
	unlink_protection_sems(data->number_of_philos);
	free(data->philos);
}
