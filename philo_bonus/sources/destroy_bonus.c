/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 23:26:24 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/20 22:38:42 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	destroy_data(t_data *data, bool should_unlink)
{
	sem_close(data->forks);
	if (should_unlink)
		sem_unlink("forks");
	free(data->philos);
}
