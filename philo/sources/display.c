/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/20 19:05:37 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/27 14:08:40 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	disp_action(unsigned int philo_nb, enum e_action action,
	struct timeval ref_time, struct timeval *needed_tv)
{
	static pthread_mutex_t	print_lock = PTHREAD_MUTEX_INITIALIZER;
	struct timeval			tv;
	long unsigned int		interval;

	pthread_mutex_lock(&print_lock);
	if (action == DIED)
		tv = *needed_tv;
	else
		gettimeofday(&tv, NULL);
	interval = (tv.tv_usec - ref_time.tv_usec)
		+ (tv.tv_sec - ref_time.tv_sec) * 1000000;
	if (action == TAKEN_A_FORK)
		printf("%lu %d has taken a fork\n", interval, philo_nb);
	if (action == THINKING)
		printf("%lu %d is thinking\n", interval, philo_nb);
	if (action == DIED)
		printf("%lu %d died\n", interval, philo_nb);
	if (action == EATING)
		printf("%lu %d is eating\n", interval, philo_nb);
	if (action == SLEEPING)
		printf("%lu %d is sleeping\n", interval, philo_nb);
	pthread_mutex_unlock(&print_lock);
}
