/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 09:37:42 by ekhaled           #+#    #+#             */
/*   Updated: 2023/10/20 19:24:33 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdint.h>
# include <pthread.h>
# include <sys/time.h>

# include "debug_colors.h"

# define INTERNAL_ERROR 1
# define ARG_ERROR 2

enum	e_action
{
	TAKEN_A_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

typedef struct s_data	t_data;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	bool			is_used;
}	t_fork;

typedef struct s_philo
{
	unsigned int	number;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	bool			has_just_slept;
	struct timeval	last_time_philo_ate;
	unsigned int	number_of_times_philo_has_eaten;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	unsigned int	number_of_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_times_each_philo_must_eat;
	unsigned int	how_many_philos_ate_enough;
	struct timeval	ref_time;
	bool			is_anyone_dead;
	t_fork			*forks;
	t_philo			*philos;
}	t_data;

bool			are_valid_params(int argc, char **argv, t_data *data);

bool			init_data(int argc, char **argv, t_data *data);

bool			ft_isdigit(char c);
int				ft_strlen(char *s);
int				ft_strcmp(char *s1, char *s2);
unsigned int	ft_atoui(char *nptr);

void			disp_action(unsigned int philo_nb, enum e_action action,
					t_data *data);

void			destroy_forks(t_fork *forks, unsigned int number_of_philos);

#endif
