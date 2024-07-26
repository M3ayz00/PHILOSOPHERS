/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/27 00:53:19 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_time(t_philo *philo)
{
	// printf("last meal : %zu\n", philo->last_meal);
	pthread_mutex_lock(philo->meal_lock);
	if (get_time() - (*philo->last_meal_p) >= philo->time_to_die)
		return (pthread_mutex_unlock(philo->meal_lock), 1);
	return (pthread_mutex_unlock(philo->meal_lock), 0);
}

int check_meals_eaten(t_observer *observer)
{
	int	i;
	t_philo	*philo;
	int	counter;

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

int	check_all_states(t_observer *observer)
{
	int	i;
	t_philo	*philo;

	philo = *(observer->philo);
	i = -1;
	while (++i < philo[0].nb_of_philo)
	{
		if (is_starving(&philo[i]))
		{
			print_msg(&philo[i], "died."RESET, RED);
			pthread_mutex_lock(philo[i].dead_lock);
			*(philo[i].dead_flag) = 1;
			return (pthread_mutex_unlock(philo[i].dead_lock), 0);
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
	if (philo->time_to_eat + philo->time_to_sleep < philo->time_to_die && philo->nb_of_philo > 2)
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
	// printf("philo %d opla\n", philo->id);
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


void	eat(t_philo *philo)
{
	if (pick_up_forks(philo))
		return ;
	if (!is_dead(philo))
	{
		pthread_mutex_lock(philo->eating_lock);
		(*philo->eating_p) = 1;
		pthread_mutex_unlock(philo->eating_lock);
		print_msg(philo, "is eating" RESET, CYAN);
		pthread_mutex_lock(philo->meal_lock);
		(*philo->meals_eaten_p)++;
		(*philo->last_meal_p) = get_time();
		pthread_mutex_unlock(philo->meal_lock);
		if (!is_dead(philo))
			ft_usleep(philo->time_to_eat);
		pthread_mutex_lock(philo->eating_lock);
		(*philo->eating_p) = 0;
		pthread_mutex_unlock(philo->eating_lock);
		put_down_forks(philo);
	}
}

void    *philo_routine(void *param)
{
	t_philo philo;

	philo = (*((t_philo *)param));
	while(1)
	{
		if (is_dead(&philo))
			break;
		eat(&philo);
		sleeep(&philo);
		think(&philo);
	}
	return (NULL);
}

void	*observer_routine(void *param)
{
	t_observer	*observer;

	observer = (*((t_observer **)param));
	while (1)
	{
		if (!check_all_states(observer) || !check_meals_eaten(observer))
			break;
	}
	return (NULL);
}

void	free_all(t_data *data)
{
	int	n_ph;

	n_ph = data->philo->nb_of_philo;
	free(data->philo);
	free_destroy(data->fork, n_ph);
	free(data->observer);
	free(data->args);
}

int main(int ac, char **av)
{
	t_data	data;

	data = (t_data){0};
	if (ac < 5 || ac > 6)
		return (1);
	if (init_program(av, &data))
		return (1);
	free_all(&data);
}