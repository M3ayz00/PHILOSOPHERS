/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:27:53 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/27 18:37:37 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	destroy_exit(char *err)
{
	printf("%s", err);
	return (1);
}

void	free_destroy(pthread_mutex_t *fork, int nb_of_philo)
{
	int	i;

	i = -1;
	while (++i < nb_of_philo)
		pthread_mutex_destroy(&fork[i]);
	free(fork);
}

int	create_and_join(t_args *args, t_observer **observer)
{
	t_philo	*philo;
	int		i;

	philo = *((*observer)->philo);
	i = -1;
	while (++i < args->nb_of_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, &philo_routine, &philo[i]))
			return (print_error("Thread creation error.\n"));
		ft_usleep(1);
	}
	if (pthread_create(&(*observer)->tid, NULL, &observer_routine, observer))
		return (destroy_exit("Thread creation error.\n"));
	i = -1;
	while (++i < args->nb_of_philo)
	{
		if (pthread_join(philo[i].tid, NULL))
			return (print_error("Thread joining error.\n"));
	}
	if (pthread_join((*observer)->tid, NULL))
		return (destroy_exit("Thread joining error.\n"));
	return (0);
}

int	lone_philo(t_philo *philo, pthread_mutex_t *fork)
{
	ft_usleep(philo->time_to_die);
	print_msg(philo, "died" RESET, RED);
	pthread_mutex_lock(philo->dead_lock);
	*(philo->dead_flag) = 1;
	pthread_mutex_unlock(philo->dead_lock);
	pthread_mutex_unlock(fork);
	return (EXIT_FAILURE);
}

int	pick_correct_fork(pthread_mutex_t *fork1,
	pthread_mutex_t *fork2, t_philo *philo)
{
	pthread_mutex_lock(fork2);
	print_msg(philo, "has taken a fork" RESET, MAGENTA);
	if (philo->nb_of_philo == 1)
		return (lone_philo(philo, fork2));
	if (!is_dead(philo))
	{
		pthread_mutex_lock(fork1);
		print_msg(philo, "has taken a fork" RESET, MAGENTA);
		return (EXIT_SUCCESS);
	}
	pthread_mutex_unlock(fork2);
	return (EXIT_FAILURE);
}
