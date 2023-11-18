/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/16 09:37:42 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/18 13:35:29 by ekhaled          ###   ########.fr       */
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
	bool			is_taken;
}	t_fork;

typedef struct s_philo
{
	unsigned int	number;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	struct timeval	last_time_philo_ate;
	unsigned int	number_of_times_philo_has_eaten;
	t_data			*data;
}	t_philo;

typedef struct s_sim_status
{
	pthread_mutex_t	mutex;
	bool			should_sim_stop;
}	t_sim_status;

typedef struct s_data
{
	unsigned int	number_of_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_times_each_philo_must_eat;
	unsigned int	number_of_philos_that_ate_enough;
	struct timeval	ref_time;
	t_sim_status	sim_status;
	pthread_mutex_t	sim_start_mutex;
	t_fork			*forks;
	t_philo			*philos;
}	t_data;

bool			are_valid_params(int argc, char **argv);

bool			init_data(int argc, char **argv, t_data *data);

bool			start_sim(t_data *data);

void			*sim_philo_routine(void *arg);

unsigned int	timeval_to_ms(struct timeval tv);
bool			take_fork(t_fork *fork);
void			drop_fork(t_fork *fork);
bool			should_sim_stop(t_philo *philo);
bool			complete_action(t_philo *philo, unsigned int time_for_action);

bool			ft_isdigit(char c);
int				ft_strlen(char *s);
int				ft_strcmp(char *s1, char *s2);
unsigned int	ft_atoui(char *nptr);

void			safe_disp_action(unsigned int philo_nb, enum e_action action,
					t_data *data, struct timeval *tv);
void			disp_action(unsigned int philo_nb, enum e_action action,
					t_data *data, struct timeval *tv);

void			destroy_all(t_data *data);

#endif
