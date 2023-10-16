/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 09:37:42 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/17 00:08:45 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>
# include <pthread.h>

# define INTERNAL_ERROR 1
# define ARG_ERROR 2

typedef struct s_philo
{
	unsigned int	number_of_times_philosopher_has_eaten;
	pthread_t		thread;
	pthread_mutex_t	right_fork;
	pthread_mutex_t	left_fork;
}	t_philo;

typedef struct s_data
{
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_times_each_philosopher_must_eat;
	unsigned int	number_of_philosophers;
	t_philo			philos[];
}	t_data;

bool	are_valid_params(int argc, char **argv, t_data *data);

bool	ft_isdigit(char c);
int		ft_strlen(char *s);
int		ft_strcmp(char *s1, char *s2);


#endif
