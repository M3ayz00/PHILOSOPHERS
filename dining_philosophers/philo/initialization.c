/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:22:54 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/23 20:23:16 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	destroy_exit(char *err)
{
	printf("%s", err);
	return (1);
}

static int	init_observer(t_observer *observer, t_philo *philo, int nb_of_philo)
{
	if (pthread_mutex_init(&observer->dead_lock, NULL)
		|| pthread_mutex_init(&observer->write_lock, NULL)
		|| pthread_mutex_init(&observer->meal_lock, NULL)
		|| pthread_mutex_init(&observer->eating_lock, NULL))
		return (print_error("Mutex initialization error.\n"));
	observer->philo = (t_philo *)philo;
	observer->dead_flag = 0;
	observer->is_full = meals_arr(nb_of_philo);
	if (!observer->is_full)
		return (print_error("Allocation failure.\n"));
	else
		return (0);
}

static int	init_philo(t_philo *philo, t_args *args, t_observer *observer)
{
	int	temp;

	temp = 0;
	philo->nb_of_philo = args->nb_of_philo;
	philo->time_to_die = args->time_to_die;
	philo->time_to_eat = args->time_to_eat;
	philo->time_to_sleep = args->time_to_sleep;
	philo->eating = &temp;
	philo->meals_eaten = 0;
	philo->start_time = get_time();
	philo->last_meal = get_time();
	if (philo->start_time == 0 || philo->last_meal == 0)
		return (print_error("Error : gettimeofday().\n"));
	philo->is_full = observer->is_full;
	philo->dead_flag = &observer->dead_flag;
	philo->dead_lock = &observer->dead_lock;
	philo->meal_lock = &observer->meal_lock;
	philo->write_lock = &observer->write_lock;
	philo->eating_lock = &observer->eating_lock;
	return (0);
}

static int	init_forks(pthread_mutex_t *fork, int nb_of_philo)
{
	int	i;

	i = -1;
	while (++i < nb_of_philo)
	{
		if (pthread_mutex_init(&fork[i], NULL))
			return (print_error("Mutex initializationn error.\n"));
	}
	return (0);
}

static int	init_philos(t_philo *philo, t_args *args,
	t_observer *observer, pthread_mutex_t *fork)
{
	int	i;

	i = -1;
	while (++i < args->nb_of_philo)
	{
		if (init_philo(&philo[i], args, observer))
			return (1);
		philo[i].id = i + 1;
		if (args->av)
			philo[i].meals_to_eat = args->meals_to_eat;
		else
			philo[i].meals_to_eat = -1;
		philo[i].r_fork = &fork[i];
		if (i == philo[i].nb_of_philo - 1)
			philo[i].l_fork = &fork[0];
		else
			philo[i].l_fork = &fork[i + 1];
	}
	return (0);
}

int	create_and_join(t_philo *philo, t_args *args, t_observer *observer)
{
	int	i;

	i = -1;
	while (++i < args->nb_of_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, &philo_routine, &philo[i]))
			return (print_error("Thread creation error.\n"));
	}
	if (pthread_create(&observer->tid, NULL, &observer_routine, observer))
		return (destroy_exit("Thread creation error.\n"));
	i = -1;
	while (++i < args->nb_of_philo)
	{
		if (pthread_join(philo[i].tid, NULL))
			return (print_error("Thread joining error.\n"));
	}
	if (pthread_join(observer->tid, NULL))
		return (destroy_exit("Thread joining error.\n"));
	return (0);
}

int	init_program(char **av)
{
	t_args			*args;
	t_observer		observer;
	t_philo			philo[MAX_PHILO];
	pthread_mutex_t	fork[MAX_PHILO];

	args = parse_args(av);
	if (!args)
		return (1);
	if (args->nb_of_philo > MAX_PHILO)
		return (free(args), print_error("Number of philosophers exceeded.\n"));
	if (init_observer(&observer, philo, args->nb_of_philo))
		return (free(args), 1);
	if (init_forks(fork, args->nb_of_philo))
		return (free(args), 1);
	if (init_philos(philo, args, &observer, fork))
		return (free(args), 1);
	if (create_and_join(philo, args, &observer))
		return (free(args), 1);
	return (0);
}