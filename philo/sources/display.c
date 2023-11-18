/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 19:05:37 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/18 18:18:51 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	disp_action(unsigned int philo_nb, enum e_action action,
	t_data *data, struct timeval *needed_tv)
{
	struct timeval			tv;
	unsigned int			interval;

	if (action == DIED)
		tv = *needed_tv;
	else
		gettimeofday(&tv, NULL);
	interval = timeval_to_ms(tv) - timeval_to_ms(data->ref_time);
	if (action == TAKEN_A_FORK)
		printf("%u %d has taken a fork\n", interval, philo_nb);
	if (action == THINKING)
		printf("%u %d is thinking\n", interval, philo_nb);
	if (action == DIED)
		printf("%u %d died\n", interval, philo_nb);
	if (action == EATING)
		printf("%u %d is eating\n", interval, philo_nb);
	if (action == SLEEPING)
		printf("%u %d is sleeping\n", interval, philo_nb);
}

void	safe_disp_action(unsigned int philo_nb, enum e_action action,
	t_data *data, struct timeval *needed_tv)
{
	pthread_mutex_lock(&data->sim_status.mutex);
	if (data->sim_status.should_sim_stop)
	{
		pthread_mutex_unlock(&data->sim_status.mutex);
		return ;
	}
	disp_action(philo_nb, action, data, needed_tv);
	pthread_mutex_unlock(&data->sim_status.mutex);
}
