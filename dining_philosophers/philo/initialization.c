/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:22:54 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/26 22:43:19 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	destroy_exit(char *err)
{
	printf("%s", err);
	return (1);
}

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
		return (print_error("Mutex initialization error.\n"), free(observer), NULL);
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

void	free_destroy(pthread_mutex_t *fork, int nb_of_philo)
{
	int	i;

	i = -1;
	while (++i < nb_of_philo)
		pthread_mutex_destroy(&fork[i]);
	free(fork);
}

static pthread_mutex_t	*init_forks(int nb_of_philo)
{
	pthread_mutex_t	*fork;
	int	i;

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
	int	i;

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

int	create_and_join(t_args *args, t_observer **observer)
{
	int	i;
	t_philo *philo;

	philo = *((*observer)->philo);
	i = -1;
	while (++i < args->nb_of_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, &philo_routine, &philo[i]))
			return (print_error("Thread creation error.\n"));
	}
	if (pthread_create(&(*observer)->tid, NULL, &observer_routine, observer))
		return (destroy_exit("Thread creation error.\n"));
	i = -1;
	while (++i < args->nb_of_philo)
	{
		if (pthread_join(philo[i].tid, NULL))
			return (print_error("Thread joining error.\n"));
		usleep(200);
	}
	if (pthread_join((*observer)->tid, NULL))
		return (destroy_exit("Thread joining error.\n"));
	return (0);
}

int	init_program(char **av, t_data *data)
{
	t_args			*args;
	t_observer		*observer;
	t_philo			*philo;
	pthread_mutex_t	*fork;

	observer = NULL;
	philo = NULL;
	fork = NULL;
	args = parse_args(av);
	if (!args)
		return (1);
	observer = init_observer();
	if (!observer)
		return (free(args), 1);
	fork = init_forks(args->nb_of_philo);
	if (!fork)
		return (free(args), free(observer), 1);
	philo = init_philos(args, &observer, &fork);
	if (!philo)
		return (free(args),
			free_destroy(fork, args->nb_of_philo), free(observer),1);
	observer->philo = &philo;
	if (create_and_join(args, &observer))
		return (free(args),  free(philo), free(observer),
			free_destroy(fork, args->nb_of_philo), 1);
	data->args = args;
	data->fork = fork;
	data->observer = observer;
	data->philo = philo;
	return (0);
}
