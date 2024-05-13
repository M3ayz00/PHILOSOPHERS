/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/05/13 15:56:39 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	destroy_exit(char *err)
{
	printf("%s", err);
	return (1);
}

size_t  get_time(void)
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL) == -1)
		exit(1);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);   
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time();
	while ((get_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

int check_meals_eaten(t_philo *philo)
{
	int i;
	int counter;

	i = -1;
	counter = 0;
	if (philo[0].meals_to_eat == -1)
		return (0);
	while (++i < philo[0].nb_of_philo)
	{
		pthread_mutex_lock(&philo[i].meal_lock);
		if (philo[i].meals_to_eat >= philo[i].meals_eaten)
			counter++;
		pthread_mutex_unlock(&philo[i].meal_lock);
	}
	if (counter == philo[0].nb_of_philo)
		return (1);
	else
		return (0);
}

int check_dead_philo(t_philo *philo)
{
	int i;
	int j;

	i = -1;
	while (++i < philo[0].nb_of_philo)
	{
		// printf("dead for %d : %d\n", i, philo[i].dead);
		pthread_mutex_lock(&philo[i].dead_lock);
		if (philo[i].dead)
		{
			j = -1;
			while (++j < philo[0].nb_of_philo)
				philo[j].dead = 1;
			return (1);
		}
		pthread_mutex_unlock(&philo[i].dead_lock);
	}
	return (0);
}

void    sleeping(t_philo *philo)
{
	printf("%ld %d is sleeping\n", get_time(), philo->id);
	usleep(philo->time_to_sleep * 100);
}

void    thinking(int id)
{
	printf("%ld %d is thinking.\n", get_time(), id);
}

void    eating(t_philo *philo, int id)
{
	pthread_mutex_lock(&philo[id].meal_lock);
	philo[id].eating = 1;
	pthread_mutex_lock(&philo[id].fork);
	if (!philo[(id + 1) % philo[id].nb_of_philo].eating)
	{
		printf("%ld %d has taken a fork.\n",get_time(), id + 1);
		pthread_mutex_lock(&philo[(id + 1)% philo[id].nb_of_philo].fork);
		printf("%ld %d has taken a fork.\n", get_time(), id + 1);
		printf("%ld %d is eating.\n", get_time(), id + 1);
		usleep(philo[id].time_to_eat * 1000);
		philo[id].meals_eaten++;
		philo[id].eating = 0;
		pthread_mutex_unlock(&philo[(id + 1)% philo[id].nb_of_philo].fork);
		pthread_mutex_unlock(&philo[id].fork);
	}
	else
	{
		pthread_mutex_unlock(&philo[id].fork);
		philo[id].eating = 0;
	}
	pthread_mutex_unlock(&philo[id].meal_lock);
} 

void    *philo_routine(void *param)
{
	t_philo *philo;
	int     id;

	philo = ((t_philo *)param);
	id = (*philo).id - 1;
	while(!check_dead_philo(philo))
	{
		eating(philo, id);
		sleeping(&philo[id]);
		thinking(id + 1);
	}
	return (NULL);
}

int    init_program(char **av)
{
	t_philo philo[MAX_PHILO];

	if (atoi(av[1]) > MAX_PHILO)
		return (destroy_exit("Number of philosophers exceeded.\n"));
	int i = -1;
	while (++i < atoi(av[1]))
	{
		philo[i].nb_of_philo = atoi(av[1]);
		philo[i].id = i + 1;
		philo[i].time_to_die = atoi(av[2]);
		philo[i].time_to_sleep = atoi(av[3]);
		philo[i].time_to_eat = atoi(av[4]);
		if (av[5])
			philo[i].meals_to_eat = atoi(av[5]);
		else
			philo[i].meals_to_eat = -1;
		philo[i].dead = 0;
		philo[i].eating = 0;
		philo[i].meals_eaten = 0;
		pthread_mutex_init(&philo[i].fork, NULL);
		pthread_mutex_init(&philo[i].dead_lock, NULL);
		pthread_mutex_init(&philo[i].meal_lock, NULL);
	}
	i = -1;
	while (++i < philo[0].nb_of_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, &philo_routine, (void *)&philo[i]))
			return (destroy_exit("Thread creation error.\n"));
	}
	i = -1;
	while (++i < philo[0].nb_of_philo)
	{	
		if (pthread_join(philo[i].tid, NULL))
			return (destroy_exit("Thread joining error.\n"));
	}
	return (0);
}

int main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (1);
	if (init_program(av))
		return (1);
}