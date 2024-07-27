/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 18:06:47 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/27 18:17:00 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	pick_up_forks(t_philo *philo)
{
	if (!is_dead(philo) && !is_starving(philo))
	{
		if (philo->id % 2 != 0)
			return (pick_correct_fork(philo->l_fork, philo->r_fork, philo));
		return (pick_correct_fork(philo->r_fork, philo->l_fork, philo));
	}
	if (is_starving(philo))
		kill_philo(philo);
	return (EXIT_FAILURE);
}

static void	put_down_forks(t_philo *philo)
{
	if (philo->id % 2 != 0)
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}

void	sleeep(t_philo *philo)
{
	print_msg(philo, "is sleeping" RESET, BLUE);
	if (!is_dead(philo))
		ft_usleep(philo->time_to_sleep);
}

void	think(t_philo *philo)
{
	print_msg(philo, "is thinking" RESET, GREEN);
	if (philo->time_to_eat + philo->time_to_sleep < philo->time_to_die
		&& philo->nb_of_philo > 2)
		ft_usleep(5);
}

void	eat(t_philo *philo)
{
	if (pick_up_forks(philo))
		return ;
	print_msg(philo, "is eating" RESET, CYAN);
	pthread_mutex_lock(philo->meal_lock);
	(*philo->meals_eaten_p)++;
	(*philo->last_meal_p) = get_time();
	pthread_mutex_unlock(philo->meal_lock);
	pthread_mutex_lock(philo->eating_lock);
	(*philo->eating_p) = 1;
	pthread_mutex_unlock(philo->eating_lock);
	if (!is_dead(philo))
		ft_usleep(philo->time_to_eat);
	pthread_mutex_lock(philo->eating_lock);
	(*philo->eating_p) = 0;
	pthread_mutex_unlock(philo->eating_lock);
	put_down_forks(philo);
}
