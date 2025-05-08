/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/27 18:43:53 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	{
		eat(&philo);
		think(&philo);
		sleeep(&philo);
	}
	return (NULL);
}

void	*observer_routine(void	*param)
{
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
}

int	main(int ac, char **av)
{
	t_data	data;

	data = (t_data){0};
	if (ac < 5 || ac > 6)
		return (print_error("Usage : ./philo nb_of_philo "),
			print_error("time_to_die "),
			print_error("time_to_eat "),
			print_error("time_to_sleep "),
			print_error("[nb_of_meals_to_eat]\n"));
	if (init_program(av, &data))
		return (1);
	free_all(&data);
}
