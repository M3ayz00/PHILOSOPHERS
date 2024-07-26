/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 17:27:53 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/27 00:53:02 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	printf("philo %d opla\n", philo->id);
	pthread_mutex_unlock(fork2);
	return (EXIT_FAILURE);
}