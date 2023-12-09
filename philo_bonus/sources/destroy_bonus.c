/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 23:26:24 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/09 12:02:46 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"


void	unlink_protection_sems(unsigned int number_of_philos,
	enum e_protection_sem_type sem_type)
{
	unsigned int	i;
	char			c;

	if (sem_type == ACCESS_SEM)
		c = 'a';
	if (sem_type == PRINT_SEM)
		c = 'p';
	if (sem_type == STATUS_SEM)
		c = 's';
	i = 0;
	while (i < number_of_philos)
	{
		sem_unlink((char [4]){'/', c, i + '0', '\0'});
		i++;
	}
}

void	unlink_sems(unsigned int number_of_philos)
{
	sem_unlink("/forks");
	sem_unlink("/death_print_protection_sem");
	unlink_protection_sems(number_of_philos, PRINT_SEM);
	unlink_protection_sems(number_of_philos, ACCESS_SEM);
	unlink_protection_sems(number_of_philos, STATUS_SEM);
}

void	destroy_data(t_data *data, bool should_unlink)
{
	sem_close(data->forks);
	sem_close(data->death_print_protection_sem);
	if (should_unlink)
		unlink_sems(data->number_of_philos);
	free(data->philos);
}
