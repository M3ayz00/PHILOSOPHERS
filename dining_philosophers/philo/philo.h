/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:47:56 by msaadidi          #+#    #+#             */
/*   Updated: 2024/04/29 18:56:37 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define MAX_PHILO 200
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

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
    int             dead;
    int             eating;
    pthread_mutex_t *fork;
    pthread_mutex_t *dead_lock;
    pthread_mutex_t *meal_lock;
}   t_philo;

#endif