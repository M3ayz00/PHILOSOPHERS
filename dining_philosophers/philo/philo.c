/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/23 18:20:38 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// int	destroy_exit(char *err)
// {
// 	printf("%s", err);
// 	return (1);
// }

// size_t  get_time(void)
// {
// 	struct timeval tv;
// 	if (gettimeofday(&tv, NULL) == -1)
// 		exit(1);
// 	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);   
// }

// int	ft_usleep(size_t milliseconds)
// {
// 	size_t	start;

// 	start = get_time();
// 	while ((get_time() - start) < milliseconds)
// 		usleep(500);
// 	return (0);
// }

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

// int	check_philo_meals(t_philo *philos)
// {
// 	int	i;
// 	int	count;

// 	i = -1;
// 	count = 0;
// 	if (philos[0].meals_to_eat == -1)
// 		return (0);
// 	while (++i < philos[0].nb_of_philo)
// 	{
// 		if (check_meals_eaten(philos[i]))
// 			count++;
// 	}
// 	return (kill_philos(philos, count));
// }


// int is_dead(t_philo *philo)
// {
// 	pthread_mutex_lock(philo->dead_lock);
// 	if (*(philo->dead_flag) == 1)
// 		return (pthread_mutex_unlock(philo->dead_lock), 1);
// 	return (pthread_mutex_unlock(philo->dead_lock), 0);
// }

// void	print_msg(t_philo *philo, char *act, char *color)
// {
// 	if (!is_dead(philo))
// 	{
// 		pthread_mutex_lock(philo->write_lock);
// 		if (!is_dead(philo))
// 			printf("%s%ld %d %s\n", color, get_time() - philo->start_time,
// 				philo->id, act);
// 		pthread_mutex_unlock(philo->write_lock);
// 	}
// }

int	check_all_states(t_observer *observer)
{
	int	i;

	i = -1;
	// printf("start: \n");
	while (++i < observer->philo[0].nb_of_philo)
	{
		// printf("is eating : %d\n", observer->philo[i].eating);
		if (observer->philo[i].eating == 0 && get_time() - observer->philo[i].last_meal >= observer->philo[i].time_to_die)
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

// int	lone_philo(t_philo *philo)
// {
// 	print_msg(philo, "died" RESET, RED);
// 	*(philo->dead_flag) = 1;
// 	pthread_mutex_unlock(philo->r_fork);
// 	return (EXIT_FAILURE);
// }

// int	pick_correct_fork(pthread_mutex_t *fork1,
// 	pthread_mutex_t *fork2, t_philo *philo)
// {
// 	pthread_mutex_lock(fork2);
// 	print_msg(philo, "has taken a fork" RESET, MAGENTA);
// 	if (philo->nb_of_philo == 1)
// 		return (lone_philo(philo));
// 	if (!is_dead(philo))
// 	{
// 		pthread_mutex_lock(fork1);
// 		print_msg(philo, "has taken a fork" RESET, MAGENTA);
// 		return (EXIT_SUCCESS);
// 	}
// 	pthread_mutex_unlock(fork2);
// 	return (EXIT_FAILURE);
// }

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
	t_observer	*observer;

	observer = (((t_observer *)param));
	while (1)
	{
		if (check_all_states(observer) == 0)
			break;
	}
	return (NULL);
}


// int    init_program(char **av)
// {
// 	t_observer			observer;
// 	t_philo 		philo[MAX_PHILO];
// 	pthread_mutex_t	fork[MAX_PHILO];

// 	if (atoi(av[1]) > MAX_PHILO)
// 		return (destroy_exit("Number of philosophers exceeded.\n"));
// 	pthread_mutex_init(&observer.dead_lock, NULL);
// 	pthread_mutex_init(&observer.write_lock, NULL);
// 	pthread_mutex_init(&observer.meal_lock, NULL);
// 	observer.philo = &philo;
// 	observer.dead_flag = 0;
// 	int i = -1;
// 	while (++i < atoi(av[1]))
// 		pthread_mutex_init(&fork[i], NULL);
// 	i = -1;
// 	while (++i < atoi(av[1]))
// 	{
// 		philo[i].nb_of_philo = atoi(av[1]);
// 		philo[i].id = i + 1;
// 		philo[i].time_to_die = atoi(av[2]);
// 		philo[i].time_to_eat = atoi(av[3]);
// 		philo[i].time_to_sleep = atoi(av[4]);
// 		if (av[5])
// 			philo[i].meals_to_eat = atoi(av[5]);
// 		else
// 			philo[i].meals_to_eat = -1;
// 		philo[i].dead = 0;
// 		philo[i].eating = 0;
// 		philo[i].meals_eaten = 0;
// 		philo[i].start_time = get_time();
// 		philo[i].last_meal = get_time();
// 		philo[i].dead_flag = &observer.dead_flag;
// 		philo[i].dead_lock = &observer.dead_lock;
// 		philo[i].meal_lock = &observer.meal_lock;
// 		philo[i].write_lock = &observer.write_lock;
// 		philo[i].r_fork = &fork[i];
// 		if (i == philo[i].nb_of_philo - 1)
// 			philo[i].l_fork = &fork[0];
// 		else
// 			philo[i].l_fork = &fork[i + 1];
// 	}
// 	i = -1;
// 	while (++i < philo[0].nb_of_philo)
// 	{
// 		if (pthread_create(&philo[i].tid, NULL, philo_routine, &philo[i]))
// 			return (destroy_exit("Thread creation error.\n"));
// 		if (philo[i].id % 2 != 0)
// 			ft_usleep(1);
// 	}
// 	if (pthread_create(&observer.tid, NULL, &observer_routine, (void *)&observer))
// 		return (destroy_exit("Thread creation error.\n"));
// 	i = -1;
// 	while (++i < philo[0].nb_of_philo)
// 	{	
// 		if (pthread_join(philo[i].tid, NULL))
// 			return (destroy_exit("Thread joining error.\n"));
// 	}
// 	if (pthread_join(observer.tid, NULL))
// 			return (destroy_exit("Thread joining error.\n"));
// 	return (0);
// }

int main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (1);
	if (init_program(av))
		return (1);
}