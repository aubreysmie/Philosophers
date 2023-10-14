/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_handling_test.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 14:53:56 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/14 15:49:20 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

void	disp_error(void)
{
	dprintf(2, "An error has occured\n");
}

int	main(void)
{
	struct timeval	tv1;
	struct timeval	tv2;
	time_t			usec_interval;
	time_t			sec_interval;
	time_t			interval;

	if (gettimeofday(&tv1, NULL) < 0)
		return (disp_error(), 1);
	if (usleep(5000000) < 0)
		return (disp_error(), 1);
	if (gettimeofday(&tv2, NULL) < 0)
		return (disp_error(), 1);
	sec_interval = tv2.tv_sec - tv1.tv_sec;
	usec_interval = tv2.tv_usec - tv1.tv_usec;
	interval = usec_interval + sec_interval * 1000000 ;
	printf("We have started this program since %lu microseconds.\n",
		interval);
}
