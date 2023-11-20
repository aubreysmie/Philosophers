/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 05:48:07 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/20 22:00:29 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sim_philo_routine(t_philo *philo)
{
	usleep(1000);//time for all processes to be created
	while (true)
	{
		if (!sim_thinking(philo))
			break ;//probably exit failure if some function failed
		if (!sim_eating(philo))
			break ;//probably exit failure if some function failed
		if (!sim_sleeping(philo))
			break ;//probably exit failure if some function failed
	}
}

bool	start_sim(t_data *data)
{
	if (!create_processes(data))
		return (0);
}
