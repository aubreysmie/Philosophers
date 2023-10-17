/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 09:37:42 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/18 00:47:50 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <pthread.h>
# include <sys/time.h>

# include "debug_colors.h"

# define INTERNAL_ERROR 1
# define ARG_ERROR 2

typedef	t_data;

typedef struct s_philo
{
	unsigned int	number;
	pthread_t		thread;
	pthread_mutex_t	right_fork;
	pthread_mutex_t	left_fork;
	struct timeval	*last_time_philo_ate;
	unsigned int	number_of_times_philosopher_has_eaten;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	unsigned int	number_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_times_each_philosopher_must_eat;
	t_philo			philos[];
}	t_data;

bool			are_valid_params(int argc, char **argv, t_data *data);

bool			ft_isdigit(char c);
int				ft_strlen(char *s);
int				ft_strcmp(char *s1, char *s2);
unsigned int	ft_atoui(char *nptr);


#endif
