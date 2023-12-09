/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 23:56:38 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/06 23:24:24 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!are_valid_params(argc, argv))
		return (ARG_ERROR);
	if (!init_data(argc, argv, &data))
		return (INTERNAL_ERROR);
	if (!start_sim(&data))
	{
		destroy_data(&data, UNLINK);
		return (INTERNAL_ERROR);
	}
	destroy_data(&data, UNLINK);
	return (0);
}
