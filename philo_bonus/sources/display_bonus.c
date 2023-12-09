/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 06:29:58 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/09 23:13:10 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	disp_action(t_philo *philo, enum e_action action,
	t_data *data, struct timeval *needed_tv)
{
	struct timeval			tv;
	unsigned int			interval;

	sem_wait(philo->data->print_protection);
	if (action == DIED)
		tv = *needed_tv;
	else
		gettimeofday(&tv, NULL);
	interval = timeval_to_ms(tv) - timeval_to_ms(data->ref_time);
	(void) interval;
	if (action == TAKEN_A_FORK)
		printf("%u %d has taken a fork\n", interval, philo->number + 1);
	if (action == THINKING)
		printf("%u %d is thinking\n", interval, philo->number + 1);
	if (action == DIED)
		printf("%u %d died\n", interval, philo->number + 1);
	if (action == EATING)
		printf("%u %d is eating\n", interval, philo->number + 1);
	if (action == SLEEPING)
		printf("%u %d is sleeping\n", interval, philo->number + 1);
	if (action != DIED)
		sem_post(philo->data->print_protection);
}
