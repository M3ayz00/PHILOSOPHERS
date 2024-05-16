/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/05/14 19:07:17 by msaadidi         ###   ########.fr       */
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

int check_meals_eaten(t_philo philo)
{
	if (philo.meals_to_eat == -1)
		return (0);
	if (philo.meals_to_eat == philo.meals_eaten)
		return (1);
	return (0);
}

// int check_dead_philo(t_philo philo)
// {
// }

void	print_msg(t_philo philo, char *act)
{
	pthread_mutex_lock(philo.write_lock);
	printf("%d %s\n", philo.id, act);
	pthread_mutex_unlock(philo.write_lock);
}

void    sleeping(t_philo philo)
{
	print_msg(philo, BLUE "is sleeping." RESET);
	ft_usleep(philo.time_to_sleep);
}

void    thinking(t_philo philo)
{
	print_msg(philo, GREEN "is thinking." RESET);
}

void    eating(t_philo philo)
{
	pthread_mutex_lock(philo.l_fork);
	print_msg(philo, MAGENTA "has taken his left fork." RESET);
	pthread_mutex_lock(philo.r_fork);
	print_msg(philo, MAGENTA "has taken his right fork." RESET);
	pthread_mutex_lock(philo.meal_lock);
	philo.meals_eaten++;
	pthread_mutex_unlock(philo.meal_lock);
	print_msg(philo, CYAN "is eating." RESET);
	ft_usleep(philo.time_to_eat);
	pthread_mutex_unlock(philo.r_fork);
	pthread_mutex_unlock(philo.l_fork);
}

void    *philo_routine(void *param)
{
	t_philo philo;

	philo = (*((t_philo *)param));
	if (philo.id % 2 == 0)
		ft_usleep(5);
	while(1)
	{
		eating(philo);
		thinking(philo);
		sleeping(philo);
	}
	return (NULL);
}

int    init_program(char **av)
{
	t_data			data;
	t_philo 		philo[MAX_PHILO];
	pthread_mutex_t	fork[MAX_PHILO];

	if (atoi(av[1]) > MAX_PHILO)
		return (destroy_exit("Number of philosophers exceeded.\n"));
	pthread_mutex_init(&data.dead_lock, NULL);
	pthread_mutex_init(&data.write_lock, NULL);
	pthread_mutex_init(&data.meal_lock, NULL);
	int i = -1;
	while (++i < atoi(av[1]))
		pthread_mutex_init(&fork[i], NULL);
	i = -1;
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
		philo[i].first_meal = get_time();
		philo[i].last_meal = get_time();
		philo[i].dead_lock = &data.dead_lock;
		philo[i].meal_lock = &data.meal_lock;
		philo[i].write_lock = &data.write_lock;
		philo[i].l_fork = &fork[i];
		if (i == 0)
			philo[i].r_fork = &fork[philo[i].nb_of_philo - 1];
		else
			philo[i].r_fork = &fork[i - 1];
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