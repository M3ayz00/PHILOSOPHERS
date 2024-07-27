/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 18:07:54 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/27 18:31:27 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_time(t_philo *philo)
{
	pthread_mutex_lock(philo->meal_lock);
	if (get_time() - (*philo->last_meal_p) >= philo->time_to_die)
		return (pthread_mutex_unlock(philo->meal_lock), 1);
	return (pthread_mutex_unlock(philo->meal_lock), 0);
}

int	is_eating(t_philo *philo)
{
	pthread_mutex_lock(philo->eating_lock);
	if (*philo->eating_p)
		return (pthread_mutex_unlock(philo->eating_lock), 1);
	return (pthread_mutex_unlock(philo->eating_lock), 0);
}

int	is_starving(t_philo *philo)
{
	return (!is_eating(philo) && is_time(philo));
}

int	check_meals_eaten(t_observer *observer)
{
	t_philo	*philo;
	int		counter;
	int		i;

	philo = *(observer->philo);
	counter = 0;
	i = -1;
	if (philo[0].meals_to_eat == -1)
		return (1);
	while (++i < philo[0].nb_of_philo)
	{
		pthread_mutex_lock(philo->meal_lock);
		if ((*philo[i].meals_eaten_p) >= philo[i].meals_to_eat)
			counter++;
		pthread_mutex_unlock(philo->meal_lock);
	}
	if (counter == philo[0].nb_of_philo)
	{
		pthread_mutex_lock(philo->dead_lock);
		(*philo->dead_flag) = 1;
		pthread_mutex_unlock(philo->dead_lock);
		return (0);
	}
	return (1);
}

int	check_all_states(t_observer *observer)
{
	t_philo	*philo;
	int		i;

	philo = *(observer->philo);
	i = -1;
	while (++i < philo[0].nb_of_philo)
	{
		if (is_starving(&philo[i]) || is_dead(philo))
		{
			print_msg(&philo[i], "died."RESET, RED);
			pthread_mutex_lock(philo[i].dead_lock);
			*(philo[i].dead_flag) = 1;
			return (pthread_mutex_unlock(philo[i].dead_lock), 0);
		}
	}
	return (1);
}
