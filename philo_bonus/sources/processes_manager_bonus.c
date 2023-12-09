/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes_manager_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 06:15:46 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/22 10:41:09 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	destroy_processes(t_philo *philos, unsigned int max_process)
{
	unsigned int	i;

	i = max_process;
	while (i)
	{
		kill(philos[i].pid, SIGKILL);
		i--;
	}
	kill(philos[i].pid, SIGKILL);
}

bool	create_processes(t_data *data)
{
	unsigned int	i;
	pid_t			cpid;

	i = 0;
	while (i < data->number_of_philos)
	{
		cpid = fork();
		if (cpid == -1)
		{
			write(2, "An internal error has occured\n", 30);
			if (i)
				destroy_processes(data->philos, i - 1);
			return (0);
		}
		if (cpid > 0)
			data->philos[i].pid = cpid;
		if (cpid == 0)
			create_philo(data->philos + i);
		i++;
	}
	return (1);
}

