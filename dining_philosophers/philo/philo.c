/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/05/20 18:32:22 by msaadidi         ###   ########.fr       */
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
	pthread_mutex_lock(philo.meal_lock);
	if (philo.meals_to_eat == philo.meals_eaten)
		return (pthread_mutex_unlock(philo.meal_lock), 1);
	return (pthread_mutex_unlock(philo.meal_lock), 0);
}

int	check_philo_meals(t_philo *philos)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	if (philos[0].meals_to_eat == -1)
		return (0);
	while (++i < philos[0].nb_of_philo)
	{
		if (check_meals_eaten(philos[i]))
			count++;
	}
	if (count == philos[0].nb_of_philo)
		return (1);
	return (0);
}


int check_dead_philo(t_philo philo)
{
	pthread_mutex_lock(philo.dead_lock);
	if (philo.dead == 1)
		return (pthread_mutex_unlock(philo.dead_lock), 1);
	return (pthread_mutex_unlock(philo.dead_lock), 0);
}


void	print_msg(t_philo philo, char *act, char *color)
{
	pthread_mutex_lock(philo.write_lock);
	printf("%s[%ld] %d %s\n", color, get_time() - *(philo.start_time), philo.id, act);
	pthread_mutex_unlock(philo.write_lock);
}

int	check_all_states(t_philo *philo)
{
	int	i;

	i = -1;
	while (++i < philo[0].nb_of_philo)
	{
		if (philo[i].last_meal >= philo[i].time_to_die && philo[i].eating == 0)
		{
			printf("last_meal : %ld, time_to_die : %ld\n", philo[i].last_meal, philo[i].time_to_die);
			print_msg(philo[i], "died."RESET, RED);
			pthread_mutex_lock(philo[0].dead_lock);
			philo[i].dead = 1;
			return (pthread_mutex_unlock(philo[0].dead_lock), 1);
		}
	}
	return (0);
}

void    sleeep(t_philo philo)
{
	print_msg(philo, "is sleeping."RESET, BLUE);
	ft_usleep(philo.time_to_sleep);
}

void    think(t_philo philo)
{
	print_msg(philo, "is thinking."RESET,  GREEN);
}

void	pick_up_forks(t_philo philo)
{
	if (philo.id % 2 == 0)
	{
		pthread_mutex_lock(philo.r_fork);
		print_msg(philo, "has taken his right fork."RESET,  MAGENTA);
		pthread_mutex_lock(philo.l_fork);
		print_msg(philo, "has taken his left fork."RESET,  MAGENTA);
	}
	else
	{
		pthread_mutex_lock(philo.l_fork);
		print_msg(philo, "has taken his left fork."RESET,  MAGENTA);
		pthread_mutex_lock(philo.r_fork);
		print_msg(philo, "has taken his right fork."RESET,  MAGENTA);	
	}
}

void    eat(t_philo philo)
{
	pick_up_forks(philo);
	pthread_mutex_lock(philo.meal_lock);
	philo.meals_eaten++;
	philo.last_meal = (get_time() - *(philo.start_time)) - philo.last_meal;
	printf("last_meal : %ld\n", philo.last_meal);
	philo.eating = 1;
	pthread_mutex_unlock(philo.meal_lock);
	print_msg(philo, "is eating."RESET, CYAN);
	ft_usleep(philo.time_to_eat);
	philo.eating = 0;
	pthread_mutex_unlock(philo.r_fork);
	pthread_mutex_unlock(philo.l_fork);
}

void    *philo_routine(void *param)
{
	t_philo philo;

	philo = (*((t_philo *)param));
	while(!check_dead_philo(philo))
	{
		eat(philo);
		sleeep(philo);
		think(philo);
	}
	return (NULL);
}

void	*observer_routine(void *param)
{
	t_data	data;

	data = (*((t_data *)param));
	while (1)
		if (check_philo_meals(data.philo) || check_all_states(data.philo))
			break;
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
	data.philo = philo;
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
		data.start_time = get_time();
		philo[i].start_time = &data.start_time;
		philo[i].last_meal = 0;
		philo[i].dead_lock = &data.dead_lock;
		philo[i].meal_lock = &data.meal_lock;
		philo[i].write_lock = &data.write_lock;
		philo[i].r_fork = &fork[i];
		if (i == philo[i].nb_of_philo - 1)
			philo[i].l_fork = &fork[0];
		else
			philo[i].l_fork = &fork[i + 1];
	}
	i = -1;
	while (++i < philo[0].nb_of_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, &philo_routine, (void *)&philo[i]))
			return (destroy_exit("Thread creation error.\n"));
	}
	if (pthread_create(&data.tid, NULL, &observer_routine, (void *)&data))
		return (destroy_exit("Thread creation error.\n"));
	i = -1;
	while (++i < philo[0].nb_of_philo)
	{	
		if (pthread_join(philo[i].tid, NULL))
			return (destroy_exit("Thread joining error.\n"));
	}
	if (pthread_join(data.tid, NULL))
			return (destroy_exit("Thread joining error.\n"));
	return (0);
}

int main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (1);
	if (init_program(av))
		return (1);
}