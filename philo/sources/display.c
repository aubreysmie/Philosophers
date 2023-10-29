/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 19:05:37 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/29 15:11:14 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	disp_action(unsigned int philo_nb, enum e_action action,
	t_data *data, struct timeval *needed_tv)
{
	static pthread_mutex_t	print_lock = PTHREAD_MUTEX_INITIALIZER;
	struct timeval			tv;
	unsigned int		interval;

	pthread_mutex_lock(&print_lock);
	if (action == DIED)
		tv = *needed_tv;
	else
		gettimeofday(&tv, NULL);
	interval = timeval_to_ms(tv) - timeval_to_ms(data->ref_time);
	pthread_mutex_lock(&data->sim_status.mutex);
	if (data->sim_status.should_sim_stop)
	{
		pthread_mutex_unlock(&data->sim_status.mutex);
		pthread_mutex_unlock(&print_lock);
		return ;
	}
	pthread_mutex_unlock(&data->sim_status.mutex);
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
	pthread_mutex_unlock(&print_lock);
}
