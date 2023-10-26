/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 19:05:37 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/26 13:00:26 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	disp_action(unsigned int philo_nb, enum e_action action,
	struct timeval ref_time)
{
	struct timeval	tv;
	static pthread_mutex_t	print_lock = PTHREAD_MUTEX_INITIALIZER;
	long unsigned int		interval;

	gettimeofday(&tv, NULL);
	interval = (tv.tv_usec - ref_time.tv_usec)
		+ (tv.tv_sec - ref_time.tv_sec) * 1000000;
	pthread_mutex_lock(&print_lock);
	if (action == TAKEN_A_FORK)
		printf("%lu %d has taken a fork\n", interval, philo_nb);
	if (action == THINKING)
		printf("%lu %d is thinking\n", interval, philo_nb);
	if (action == DIED)
		printf("%lu %d died\n", interval, philo_nb);
	if (action == EATING)
		usleep(data->time_to_eat);
		printf("%lu %d is eating\n", interval, philo_nb);
	if (action == SLEEPING)
		usleep(data->time_to_sleep);
		printf("%lu %d is sleeping\n", interval, philo_nb);
	pthread_mutex_unlock(&print_lock);
}
