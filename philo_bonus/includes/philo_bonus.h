/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 23:55:32 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/21 02:26:04 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>

# define INTERNAL_ERROR 1
# define ARG_ERROR 2

# define UNLINK 1

enum	e_exit_status
{
	ERROR_EXIT_STATUS = 1,
	DONE_EATING_EXIT_STATUS,
	DEATH_EXIT_STATUS
};

enum	e_action
{
	TAKEN_A_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

typedef struct s_data	t_data;

typedef struct s_philo
{
	unsigned int	number;
	pid_t			pid;
	pthread_t		thread;
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
	unsigned int	number_of_philos_that_ate_enough;
	struct timeval	ref_time;
	t_philo			*philos;
	sem_t			*forks;
}	t_data;

bool			are_valid_params(int argc, char **argv);

bool			ft_isdigit(char c);
int				ft_strlen(char *s);
int				ft_strcmp(char *s1, char *s2);
unsigned int	ft_atoui(char *nptr);

bool			init_data(int argc, char **argv, t_data *data);

void			destroy_data(t_data *data, bool should_unlink);

bool			start_sim(t_data *data);

bool			create_processes(t_data *data);

void			sim_philo_routine(t_philo *philo);

unsigned int	timeval_to_ms(struct timeval tv);

void			disp_action(unsigned int philo_nb, enum e_action action,
					t_data *data, struct timeval *needed_tv);

#endif