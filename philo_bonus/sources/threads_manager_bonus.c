/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_manager_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 04:09:11 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/02 11:06:25 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*perform_sim_check_routine(void *arg)
{
	t_philo	*philo;
	int		wstatus;
	int		exit_status;

	philo = (t_philo *)arg;
	exit_status = -1;
	if (waitpid(philo->pid, &wstatus, 0) == -1)
		return (NULL);
	if (WIFEXITED(wstatus))
		exit_status = WEXITSTATUS(wstatus);
	if (exit_status == ERROR_EXIT_STATUS)
	{
		destroy_processes(philo->data->philos,
			philo->data->number_of_philos - 1);
		return ((void *)INTERNAL_ERROR);
	}
	if (exit_status == DEATH_EXIT_STATUS)
	{
		destroy_processes(philo->data->philos,
			philo->data->number_of_philos - 1);
		return (NULL);
	}
	if (exit_status == DONE_EATING_EXIT_STATUS)
		return (NULL);
	return (NULL);
}

bool	join_threads(pthread_t *checking_threads,
	unsigned int max_thread, bool should_error_return)
{
	unsigned int	i;
	void			*retval;

	i = max_thread;
	while (i)
	{
		pthread_join(checking_threads[i], (void **)&retval);
		if (should_error_return && retval == (void *)INTERNAL_ERROR)
			return (0);
		i--;
	}
	pthread_join(checking_threads[i], (void **)&retval);
	if (should_error_return && retval == (void *)INTERNAL_ERROR)
		return (0);
	return (1);
}

bool	create_threads(t_data *data, pthread_t *checking_threads)
{
	unsigned int	i;

	i = 0;
	while (i < data->number_of_philos)
	{
		if (pthread_create(checking_threads + i, NULL,
				&perform_sim_check_routine, (void *)(data->philos + i)))
		{
			write(2, "An internal error has occured\n", 30);
			destroy_processes(data->philos, data->number_of_philos - 1);
			if (i)
				join_threads(checking_threads, i - 1, !RETURN_ERROR);
			return (0);
		}
		i++;
	}
	return (1);
}
