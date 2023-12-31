/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 23:55:32 by ekhaled           #+#    #+#             */
/*   Updated: 2023/12/11 15:52:33 by ekhaled          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>

# define UNLINK 1

enum	e_action
{
	TAKEN_A_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

typedef struct s_data	t_data;

typedef struct s_semaphore
{
	char	*name;
	sem_t	*semaphore;
}	t_semaphore;

typedef struct s_philo
{
	unsigned int	number;
	pid_t			pid;
	pthread_t		thread;
	struct timeval	last_time_philo_ate;
	int				number_of_times_philo_has_eaten;
	t_semaphore		access_protection;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	unsigned int	number_of_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				number_of_times_each_philo_must_eat;
	struct timeval	ref_time;
	t_philo			*philos;
	sem_t			*forks;
	sem_t			*print_protection;
	sem_t			*should_stop;
	sem_t			*meals;
}	t_data;

bool			are_valid_params(int argc, char **argv);

bool			ft_isdigit(char c);
int				ft_strlen(char *s);
int				ft_strcmp(char *s1, char *s2);
unsigned int	ft_atoui(char *nptr);

bool			init_data(int argc, char **argv, t_data *data);
bool			init_philo_sem(t_semaphore *sem, unsigned int philo_nb);

void			destroy_data(t_data *data, bool should_unlink);
void			unlink_sems(unsigned int number_of_philos);

bool			start_sim(t_data *data);

bool			create_processes(t_data *data);
void			destroy_processes(t_philo *philos, unsigned int max_process);

void			create_philo(t_philo *philo);
void			sim_philo_routine(t_philo *philo);
void			end_meals_check(t_data *data);

unsigned int	timeval_to_ms(struct timeval tv);
void			*perform_death_check_routine(void *arg);
void			wait_for_fork(t_philo *philo);

void			disp_action(t_philo *philo, enum e_action action,
					t_data *data, struct timeval *needed_tv);

#endif