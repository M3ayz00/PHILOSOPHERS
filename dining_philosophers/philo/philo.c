/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/06/11 19:09:00 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *param)
{
	t_philo	philo;

	philo = (*((t_philo *)param));
	while (!is_dead(&philo))
	{
		if (!check_philos_state(&philo))
			eat(&philo);
		if (!check_philos_state(&philo))
			sleeep(&philo);
		if (!check_philos_state(&philo))
			think(&philo);
	}
	return (NULL);
}

int	create_and_join(t_philo *philo, t_args *args)
{
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
}

int	main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (print_error("Usage : ./philo"),
			print_error(" number_of_philosophers time_to_die "),
			print_error("time_to_eat time_to_sleep "),
			print_error("[number_of_times_each_philosopher_must_eat]\n"));
	if (init_program(av))
		return (1);
}
