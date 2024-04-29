/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/04/29 19:09:58 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_meals_eaten(t_philo *philo)
{
    int i;
    int counter;

    i = -1;
    counter = 0;
    while (++i < philo[0]->nb_of_philo)
    {
        if (philo[i]->meals_to_eat >= philo[i]->meals_eaten)
            counter++;
    }
    if (counter == philo[0]->nb_of_philo)
        return (1);
    else
        return (0);
}

int check_dead_philo(t_philo *philo)
{
    int i;

    i = -1;
    while (++i < philo[0]->nb_of_philo)
    {
        if (philo[i]->dead)
        {
            
        }
    }
    return (0);
}

void    sleeping(t_philo *philo)
{
    printf("[timestamp_in_ms] %d is sleeping\n", philo->id);
    usleep(philo->time_to_sleep);
}

void    thinking(int id)
{
    printf("[timestamp_in_ms] %d is thinking\n", id);
}

void    eating(t_philo *philo1 , t_philo *philo2)
{
    pthread_mutex_lock(philo1->fork);
    printf("[timestamp_in_ms] %d has taken his fork.\n", philo1->id);
    pthread_mutex_lock(philo2->fork);
    printf("[timestamp_in_ms] %d has taken his right fork.\n", philo2->id);
    printf("[timestamp_in_ms] %d is eating\n", philo1->id);
    philo1->eating = 1;
    usleep(philo1->time_to_eat);
    philo1->meals_eaten++;
    philo1->eating = 0;
    pthread_mutex_unlock(philo1->fork);
    pthread_mutex_unlock(philo2->fork);
}

void    *philo_routine(void *param)
{
    t_philo *philo;
    int     i;
    int     j;

    philo = *((t_philo **)param);
    j = philo[0]->nb_of_philo;
    i = philo[0]->id;
    while ()
    {
        thinking(philo[i - 1]->id);
        if (!philo[i]->eating)
            eating(&philo[i - 1], &philo[(i + 1) % j - 1]);
        sleeping(philo[i - 1]-> id);
    }
}

void    init_program(char **av)
{
    t_philo philo[MAX_PHILO];

    if (atoi(av[1]) > MAX_PHILO)
        exit(1);
    int i = -1;
    philo[0]->nb_of_philo = atoi(av[1]);
    while (++i < atoi(av[1]))
    {
        philo[i]->id = i + 1;
        philo[i]->time_to_die = atoi(av[2]);
        philo[i]->time_to_sleep = atoi(av[3]);
        philo[i]->time_to_eat = atoi(av[4]);
        if (av[5])
            philo[i]->meals_to_eat = atoi(av[5]);
        else
            philo[i]->meals_to_eat = -1;
        philo[i]->dead = 0;
        philo[i]->eating = 0;
        philo[i]->meals_eaten = 0;
        pthread_create(&philo[i]->tid, NULL, &philo_routine, &philo);
        pthread_mutex_init(&philo[i]->fork, NULL);
    }
    i = -1;
    while (++i < atoi(av[1]))
        pthread_join(philo[i]->tid, NULL);
}

int main(int ac, char **av)
{
    if (ac < 4 || ac > 5)
        return (1);
    init_program(av);
}