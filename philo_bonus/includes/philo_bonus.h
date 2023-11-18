/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekhaled <ekhaled@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 23:55:32 by ekhaled           #+#    #+#             */
/*   Updated: 2023/11/18 23:20:12 by ekhaled          ###   ########.fr       */
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
# include <semaphore.h>

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

typedef struct s_philo
{
	unsigned int	number;
	pid_t			pid;
	struct timeval	last_time_philo_ate;
	unsigned int	number_of_times_philo_has_eaten;
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

#endif