/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:27:53 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/23 20:26:45 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	meals_counter(t_philo *philo)
{
	if (philo->meals_eaten == philo->meals_to_eat)
		philo->is_full[philo->id - 1] = 1;
}

static int	is_full(t_philo *philo)
{
	int	i;
	int	counter;

	i = -1;
	counter = 0;
	if (philo->meals_to_eat == -1)
		return (0);
	pthread_mutex_lock(philo->meal_lock);
	while (++i < philo->nb_of_philo)
		if (philo->is_full[i] == 1)
			counter++;
	pthread_mutex_unlock(philo->meal_lock);
	if (counter == philo->nb_of_philo)
		return (1);
	return (0);
}


int	check_philos_state(t_philo *philo)
{
	
	if (is_full(philo)
		|| ((*philo->eating) == 0
			&& get_time() - philo->last_meal >= philo->time_to_die))
	{
		print_msg(philo, "died" RESET, RED);
		pthread_mutex_lock(philo->dead_lock);
		*(philo->dead_flag) = 1;
		return (pthread_mutex_unlock(philo->dead_lock), 1);
	}
	return (0);
}

int	lone_philo(t_philo *philo)
{
	print_msg(philo, "died" RESET, RED);
	*(philo->dead_flag) = 1;
	pthread_mutex_unlock(philo->r_fork);
	return (EXIT_FAILURE);
}

int	pick_correct_fork(pthread_mutex_t *fork1,
	pthread_mutex_t *fork2, t_philo *philo)
{
	pthread_mutex_lock(fork2);
	print_msg(philo, "has taken a fork" RESET, MAGENTA);
	if (philo->nb_of_philo == 1)
		return (lone_philo(philo));
	if (!check_philos_state(philo))
	{
		pthread_mutex_lock(fork1);
		print_msg(philo, "has taken a fork" RESET, MAGENTA);
		return (EXIT_SUCCESS);
	}
	pthread_mutex_unlock(fork2);
	return (EXIT_FAILURE);
}