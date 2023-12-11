/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 23:56:38 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/11 15:52:26 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!are_valid_params(argc, argv))
		return (1);
	if (!init_data(argc, argv, &data))
		return (1);
	if (!start_sim(&data))
	{
		destroy_data(&data, UNLINK);
		return (1);
	}
	destroy_data(&data, UNLINK);
	return (0);
}
