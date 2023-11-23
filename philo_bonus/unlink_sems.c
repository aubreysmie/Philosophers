/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unlink_sems.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 11:51:20 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/23 07:25:33 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>

int	main(int argc, char **argv)
{
	int		nb_of_philos;
	int		i;
	char	*sem_name;

	if (argc != 2)
	{
		printf("No number of philos given\n");
		return (0);
	}
	sem_unlink("/forks");
	nb_of_philos = atoi(argv[1]);
	i = 0;
	sem_name = NULL;
	while (i < nb_of_philos)
	{
		sem_name = (char [3]){'/', i + '0', '\0'};
		if (sem_unlink(sem_name) == -1)
			printf("Philo %d, error : %s\n",
				i, strerror(errno));
		i++;
	}
}
