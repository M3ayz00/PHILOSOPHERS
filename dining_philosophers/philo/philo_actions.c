/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:00:53 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/19 16:45:57 by msaadidi         ###   ########.fr       */
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
	if (philo->time_to_eat + philo->time_to_sleep > philo->time_to_die && !is_dead(philo))
		ft_usleep(5);
}

static int	pick_up_forks(t_philo *philo)
{
	if (!is_dead(philo))
	{
		if (philo->id % 2 != 0)
		{
			pthread_mutex_lock(philo->r_fork);
			print_msg(philo, "has taken a fork" RESET, MAGENTA);
			if (philo->nb_of_philo == 1)
			{
				print_msg(philo, "died" RESET, RED);
				*(philo->dead_flag) = 1;
				pthread_mutex_unlock(philo->r_fork);
				return (EXIT_FAILURE);
			}
			if (!check_philos_state(philo))
			{
				pthread_mutex_lock(philo->l_fork);
				print_msg(philo, "has taken a fork" RESET, MAGENTA);
				return (EXIT_SUCCESS);
			}
			else
			{
				pthread_mutex_unlock(philo->r_fork);
				return (EXIT_FAILURE);
			}
		}
		else
		{
			pthread_mutex_lock(philo->l_fork);
			print_msg(philo, "has taken a fork" RESET, MAGENTA);
			if (!check_philos_state(philo))
			{
				pthread_mutex_lock(philo->r_fork);
				print_msg(philo, "has taken a fork" RESET, MAGENTA);
				return (EXIT_SUCCESS);
			}
			else
			{
				pthread_mutex_unlock(philo->l_fork);
				return (EXIT_FAILURE);
			}
		}
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
