/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 19:08:29 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/18 12:27:17 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (!are_valid_params(argc, argv))
		return (ARG_ERROR);
	if (!init_data(argc, argv, &data))
		return (INTERNAL_ERROR);
	if (!start_sim(&data))
	{
		destroy_all(&data);
		return (INTERNAL_ERROR);
	}
	destroy_all(&data);
	return (0);
}
