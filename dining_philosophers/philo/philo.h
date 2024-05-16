/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:47:56 by msaadidi          #+#    #+#             */
/*   Updated: 2024/05/14 18:59:41 by msaadidi         ###   ########.fr       */
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
# include <unistd.h>
# include <string.h>
#include <sys/time.h>

typedef struct s_philo
{
    pthread_t       tid;
    int             nb_of_philo;
    int             id;
    int             time_to_eat;
    int             time_to_sleep;
    int             time_to_die;
    int             meals_eaten;
    int             meals_to_eat;
    int             first_meal;
    int             last_meal;
    int             dead;
    int             eating;
    pthread_mutex_t *l_fork;
    pthread_mutex_t *r_fork;
    pthread_mutex_t *write_lock;
    pthread_mutex_t *meal_lock;
    pthread_mutex_t *dead_lock;
}   t_philo;

typedef struct s_data
{
    int             dead_flag;
    t_philo         *philo;
    pthread_mutex_t meal_lock;
    pthread_mutex_t write_lock;
    pthread_mutex_t dead_lock;
    
}   t_data;


#endif