/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/07/18 17:39:10 by msaadidi         ###   ########.fr       */
=======
/*   Updated: 2024/07/27 18:18:39 by msaadidi         ###   ########.fr       */
>>>>>>> real_checkpoint
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

<<<<<<< HEAD
void	*philo_routine(void *param)
{
	t_philo	philo;

	philo = (*((t_philo *)param));
	while (!is_dead(&philo) && !check_philos_state(&philo))
=======
int	init_program(char **av, t_data *data)
{
	t_args			*args;
	t_observer		*observer;
	t_philo			*philo;
	pthread_mutex_t	*fork;

	args = parse_args(av);
	if (!args)
		return (1);
	if (init_data(args, &observer, &philo, &fork))
		return (1);
	observer->philo = &philo;
	data->args = args;
	data->fork = fork;
	data->observer = observer;
	data->philo = philo;
	if (create_and_join(args, &observer))
		return (free_all(data), 1);
	return (0);
}

void	*philo_routine(void *param)
{
	t_philo	philo;

	philo = (*((t_philo *)param));
	while (!is_dead(&philo))
>>>>>>> real_checkpoint
	{
		if (!is_dead(&philo) || !check_philos_state(&philo))
			eat(&philo);
		if (!is_dead(&philo) || !check_philos_state(&philo))
			think(&philo);
		if (!is_dead(&philo) || !check_philos_state(&philo))
			sleeep(&philo);
	}
	return (NULL);
}

int	create_and_join(t_philo *philo, t_args *args)
{
<<<<<<< HEAD
	int	i;

	i = -1;
	while (++i < args->nb_of_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, &philo_routine, &philo[i]))
			return (print_error("Thread creation error.\n"));
	}
	i = -1;
	while (++i < args->nb_of_philo)
	{
		if (pthread_join(philo[i].tid, NULL))
			return (print_error("Thread joining error.\n"));
	}
	return (0);
=======
	t_observer	*observer;

	observer = (*((t_observer **)param));
	while (1)
	{
		if (!check_all_states(observer) || !check_meals_eaten(observer))
			break ;
	}
	return (NULL);
}

void	free_all(t_data *data)
{
	int	n_ph;

	n_ph = data->philo->nb_of_philo;
	free(data->philo);
	free_destroy(data->fork, n_ph);
	free(data->observer);
	free(data->args);
>>>>>>> real_checkpoint
}

int	main(int ac, char **av)
{
	t_data	data;

	data = (t_data){0};
	if (ac < 5 || ac > 6)
<<<<<<< HEAD
		return (print_error("Usage : ./philo"),
			print_error(" number_of_philosophers time_to_die "),
			print_error("time_to_eat time_to_sleep "),
			print_error("[number_of_times_each_philosopher_must_eat]\n"));
	if (init_program(av))
		return (system("leaks philo"), 1);
	return (system("leaks philo"), 0);
=======
		return (print_error("Usage : ./philo nb_of_philo "),
			print_error("time_to_die "),
			print_error("time_to_eat "),
			print_error("time_to_sleep "),
			print_error("[nb_of_meals_to_eat]\n"));
	if (init_program(av, &data))
		return (1);
	free_all(&data);
>>>>>>> real_checkpoint
}
