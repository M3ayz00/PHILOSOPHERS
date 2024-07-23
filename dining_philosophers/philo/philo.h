/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:47:56 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/23 17:38:14 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define MAX_PHILO 200

# define RED     "\x1b[31m"
# define GREEN   "\x1b[32m"
# define YELLOW  "\x1b[33m"
# define BLUE    "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN    "\x1b[36m"
# define RESET   "\x1b[0m"
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <string.h>
#include <sys/time.h>

typedef struct s_args
{
	int		nb_of_philo;
	int		meals_to_eat;
	int		time_to_eat;
	int		time_to_sleep;
	int		time_to_die;
	char	*av;
}	t_args;

typedef struct s_philo
{
    pthread_t       tid;
    int             nb_of_philo;
    int             id;
    size_t             time_to_eat;
    size_t             time_to_sleep;
    size_t             time_to_die;
    int             meals_eaten;
    int             meals_to_eat;
    size_t          start_time;
    size_t          last_meal;
    int             dead;
    int             eating;
    int				*is_full;
    int             *dead_flag;
    pthread_mutex_t *l_fork;
    pthread_mutex_t *r_fork;
    pthread_mutex_t *write_lock;
    pthread_mutex_t *meal_lock;
    pthread_mutex_t *dead_lock;
}   t_philo;

typedef struct s_observer
{
    pthread_t       tid;
    t_philo         *philo;
    int             dead_flag;
    int				*is_full;
    pthread_mutex_t meal_lock;
    pthread_mutex_t write_lock;
    pthread_mutex_t dead_lock;
    
}   t_observer;

int	    destroy_exit(char *err);
t_args	*parse_args(char **av);
int		print_error(char *err);
int		ft_atoi(char *str);
int		init_program(char **av);
int		*meals_arr(int nb_of_philo);
size_t	get_time(void);
int	create_and_join(t_philo *philo, t_args *args, t_observer observer);
void	sleeep(t_philo *philo);
void	think(t_philo *philo);
void	eat(t_philo *philo);
int		is_dead(t_philo *philo);
int		check_philos_state(t_philo *philo);
void	print_msg(t_philo *philo, char *act, char *color);
int		ft_usleep(size_t milliseconds);
void	meals_counter(t_philo *philo);
int		lone_philo(t_philo *philo);
void    *philo_routine(void *param);
void	*observer_routine(void *param);
int		pick_correct_fork(pthread_mutex_t *fork1,
			pthread_mutex_t *fork2, t_philo *philo);

#endif