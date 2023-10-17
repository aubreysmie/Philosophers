/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 19:08:29 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/18 00:38:35 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!are_valid_params(argc, argv, &data))
		return (ARG_ERROR);
	if (!init_data(argc, argv, &data))
		return (INTERNAL_ERROR);
	if (!start_sim(&data))
	{
		//free les philos et tout le reste
		return (INTERNAL_ERROR);
	}
	return (0);
}