/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:00:53 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/20 16:43:18 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleeep(t_philo *philo)
{
	print_msg(philo, "is sleeping" RESET, BLUE);
	if (!is_dead(philo))
		ft_usleep(philo->time_to_sleep);
}

void	think(t_philo *philo)
{
	if (!is_dead(philo))
		print_msg(philo, "is thinking" RESET, GREEN);
	if (philo->time_to_eat + philo->time_to_sleep > philo->time_to_die
		&& !is_dead(philo))
		ft_usleep(5);
}

static int	pick_up_forks(t_philo *philo)
{
	if (!is_dead(philo))
	{
		if (philo->id % 2 != 0)
			return (pick_correct_fork(philo->l_fork, philo->r_fork, philo));
		return (pick_correct_fork(philo->r_fork, philo->l_fork, philo));
	}
	return (EXIT_FAILURE);
}

static void	put_down_forks(t_philo *philo)
{
	if (philo->id % 2 != 0)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
}

void	eat(t_philo *philo)
{
	if (pick_up_forks(philo))
		return ;
	if (!is_dead(philo))
	{
		philo->eating = 1;
		print_msg(philo, "is eating" RESET, CYAN);
		pthread_mutex_lock(philo->meal_lock);
		philo->meals_eaten++;
		philo->last_meal = get_time();
		pthread_mutex_unlock(philo->meal_lock);
		meals_counter(philo);
		if (!is_dead(philo))
			ft_usleep(philo->time_to_eat);
		philo->eating = 0;
		put_down_forks(philo);
	}
}
