/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:22:54 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/27 18:37:15 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static t_observer	*init_observer(void)
{
	t_observer	*observer;

	observer = (t_observer *)malloc(sizeof(t_observer));
	if (!observer)
		return (print_error("Observer allocation error.\n"), NULL);
	if (pthread_mutex_init(&observer->dead_lock, NULL)
		|| pthread_mutex_init(&observer->write_lock, NULL)
		|| pthread_mutex_init(&observer->meal_lock, NULL)
		|| pthread_mutex_init(&observer->eating_lock, NULL))
		return (print_error("Mutex initialization error.\n"),
			free(observer), NULL);
	observer->dead_flag = 0;
	return (observer);
}

static int	init_philo(t_philo *philo, t_args *args, t_observer **observer)
{
	philo->nb_of_philo = args->nb_of_philo;
	philo->time_to_die = args->time_to_die;
	philo->time_to_eat = args->time_to_eat;
	philo->time_to_sleep = args->time_to_sleep;
	philo->eating = 0;
	philo->eating_p = &philo->eating;
	philo->meals_eaten = 0;
	philo->meals_eaten_p = &philo->meals_eaten;
	philo->start_time = get_time();
	philo->last_meal = get_time();
	philo->last_meal_p = &philo->last_meal;
	if (philo->start_time == 0 || philo->last_meal == 0)
		return (print_error("Error : gettimeofday().\n"), 1);
	philo->dead_flag = &(*observer)->dead_flag;
	philo->dead_lock = &(*observer)->dead_lock;
	philo->meal_lock = &(*observer)->meal_lock;
	philo->write_lock = &(*observer)->write_lock;
	philo->eating_lock = &(*observer)->eating_lock;
	return (0);
}

static pthread_mutex_t	*init_forks(int nb_of_philo)
{
	pthread_mutex_t	*fork;
	int				i;

	i = -1;
	fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * nb_of_philo);
	if (!fork)
		return (print_error("Mutex allocation error.\n"), NULL);
	while (++i < nb_of_philo)
	{
		if (pthread_mutex_init(&fork[i], NULL))
			return (free_destroy(fork, nb_of_philo),
				print_error("Mutex initializationn error.\n"), NULL);
	}
	return (fork);
}

static t_philo	*init_philos(t_args *args,
	t_observer **observer, pthread_mutex_t **fork)
{
	t_philo	*philo;
	int		i;

	i = -1;
	philo = (t_philo *)malloc(sizeof(t_philo) * args->nb_of_philo);
	if (!philo)
		return (print_error("Philo allocation error.\n"),
			NULL);
	while (++i < args->nb_of_philo)
	{
		if (init_philo(&philo[i], args, observer))
			return (NULL);
		philo[i].id = i + 1;
		philo[i].meals_to_eat = args->meals_to_eat;
		philo[i].r_fork = &(*fork)[i];
		if (i == philo[i].nb_of_philo - 1)
			philo[i].l_fork = &(*fork)[0];
		else
			philo[i].l_fork = &(*fork)[i + 1];
	}
	return (philo);
}

int	init_data(t_args *args, t_observer **observer,
	t_philo **philo, pthread_mutex_t **fork)
{
	(*observer) = NULL;
	(*philo) = NULL;
	(*fork) = NULL;
	(*observer) = init_observer();
	if (!(*observer))
		return (free(args), 1);
	(*fork) = init_forks(args->nb_of_philo);
	if (!(*fork))
		return (free(args), free((*observer)), 1);
	(*philo) = init_philos(args, &(*observer), &(*fork));
	if (!(*philo))
		return (free(args),
			free_destroy((*fork), args->nb_of_philo),
			free((*observer)), 1);
	return (0);
}
