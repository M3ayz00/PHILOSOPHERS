/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:05:08 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/18 17:36:35 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_philo *philo)
{
	if (*(philo->dead_flag) == 1)
		return (1);
	return (0);
}

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
		|| (philo->eating == 0
			&& get_time() - philo->last_meal >= philo->time_to_die))
	{
		print_msg(philo, "died" RESET, RED);
		pthread_mutex_lock(philo->dead_lock);
		*(philo->dead_flag) = 1;
		return (pthread_mutex_unlock(philo->dead_lock), 1);
	}
	return (0);
}
