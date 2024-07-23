/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/23 20:29:10 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_meals_eaten(t_philo philo)
{
	pthread_mutex_lock(philo.meal_lock);
	if (philo.meals_to_eat == philo.meals_eaten)
		return (pthread_mutex_unlock(philo.meal_lock), 1);
	return (pthread_mutex_unlock(philo.meal_lock), 0);
}

int	kill_philos(t_philo *philos, int count)
{
	int i = -1;
	if (count == philos[0].nb_of_philo)
	{
		while (++i < philos[0].nb_of_philo)
			pthread_detach(philos[i].tid);
		return (1);
	}
	return (0);
}

void	kill_philos_2(t_philo *philos)
{
	int i = -1;
	while (++i < philos[0].nb_of_philo)
		pthread_detach(philos[i].tid);
}

int	is_eating(t_philo *philo)
{
	pthread_mutex_lock(philo->eating_lock);
	if ((*philo->eating))
		return (pthread_mutex_unlock(philo->eating_lock), 1);
	return (pthread_mutex_unlock(philo->eating_lock), 0);
}	

int	check_all_states(t_observer *observer)
{
	int	i;

	i = -1;
	// printf("start: \n");
	while (++i < observer->philo[0].nb_of_philo)
	{
		// printf("is eating : %d\n", observer->philo[i].eating);
		if (!is_eating(&(observer->philo[i])) && get_time() - observer->philo[i].last_meal >= observer->philo[i].time_to_die)
		{
			print_msg(&observer->philo[i], "died."RESET, RED);
			pthread_mutex_lock(observer->philo[i].dead_lock);
			*(observer->philo[i].dead_flag) = 1;
			return (pthread_mutex_unlock(observer->philo[i].dead_lock), 0);
		}
	}
	return (1);
}

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
		pthread_mutex_lock(philo->eating_lock);
		(*philo->eating) = 1;
		pthread_mutex_unlock(philo->eating_lock);
		print_msg(philo, "is eating" RESET, CYAN);
		pthread_mutex_lock(philo->meal_lock);
		philo->meals_eaten++;
		philo->last_meal = get_time();
		pthread_mutex_unlock(philo->meal_lock);
		meals_counter(philo);
		if (!is_dead(philo))
			ft_usleep(philo->time_to_eat);
		pthread_mutex_lock(philo->eating_lock);
		(*philo->eating) = 0;
		pthread_mutex_unlock(philo->eating_lock);
		put_down_forks(philo);
	}
}

void    *philo_routine(void *param)
{
	t_philo philo;

	philo = (*((t_philo *)param));
	while(!is_dead(&philo))
	{
		eat(&philo);
		sleeep(&philo);
		think(&philo);
	}
	return (NULL);
}

void	*observer_routine(void *param)
{
	t_observer	observer;

	observer = (*((t_observer *)param));
	while (1)
	{
		if (check_all_states(&observer) == 0)
			break;
	}
	return (NULL);
}

int main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (1);
	if (init_program(av))
		return (1);
}