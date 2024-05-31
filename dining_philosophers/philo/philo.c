/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 01:49:29 by msaadidi          #+#    #+#             */
/*   Updated: 2024/05/31 17:54:57 by msaadidi         ###   ########.fr       */
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

int	check_philos_meals(t_philo *philos)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	if (philos[0].meals_to_eat == -1)
		return (1);
	while (++i < philos[0].nb_of_philo)
	{
		if (check_meals_eaten(philos[i]))
			count++;
	}
	if (count == philos[0].nb_of_philo)
	{	
		*(philos[0].dead_flag) = 1;
		return (0);
	}
	return (1);
}


int is_dead(t_philo *philo)
{
	pthread_mutex_lock(philo->dead_lock);
	if (*(philo->dead_flag) == 1)
		return (pthread_mutex_unlock(philo->dead_lock), 1);
	return (pthread_mutex_unlock(philo->dead_lock), 0);
}


void	print_msg(t_philo *philo, char *act, char *color)
{
	if (is_dead(philo))
		return;
	pthread_mutex_lock(philo->write_lock);
	printf("%s[%ld] %d %s\n", color, get_time() - philo->start_time, philo->id, act);
	pthread_mutex_unlock(philo->write_lock);
}

int	is_full(t_philo *philo)
{
	int	i;
	int	counter;

	i = -1;
	counter = 0;
	pthread_mutex_lock(philo->meal_lock);
	while (i++ < philo->nb_of_philo)
		if (philo->is_full[i] == 1)
			counter++;
	pthread_mutex_unlock(philo->meal_lock);
	if (counter == philo->nb_of_philo)
		return (1);
	return (0);
}

int	check_philos_state(t_philo *philo, int id)
{
	if (is_full(philo) || (philo->eating == 0 && get_time() - philo->last_meal >= philo->time_to_die))
	{
		printf("philo id %d\n", id);
		print_msg(philo, "died."RESET, RED);
		pthread_mutex_lock(philo->dead_lock);
		*(philo->dead_flag) = 1;
		return (pthread_mutex_unlock(philo->dead_lock), 1);
	}
	return (0);
}

void    sleeep(t_philo *philo)
{
	print_msg(philo, "is sleeping."RESET, BLUE);
	ft_usleep(philo->time_to_sleep);
}

void    think(t_philo *philo)
{
	print_msg(philo, "is thinking."RESET,  GREEN);
}

void	pick_up_forks(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->r_fork);
		print_msg(philo, "has taken his right fork."RESET,  MAGENTA);
		pthread_mutex_lock(philo->l_fork);
		print_msg(philo, "has taken his left fork."RESET,  MAGENTA);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		print_msg(philo, "has taken his left fork."RESET,  MAGENTA);
		pthread_mutex_lock(philo->r_fork);
		print_msg(philo, "has taken his right fork."RESET,  MAGENTA);
	}	
}

void	meals_counter(t_philo *philo)
{
	if (philo->meals_eaten == philo->meals_to_eat)
		*(philo->is_full + philo->id - 1) = 1;
}

void    eat(t_philo *philo)
{
	pick_up_forks(philo);
	philo->eating = 1;
	print_msg(philo, "is eating."RESET, CYAN);
	pthread_mutex_lock(philo->meal_lock);
	philo->meals_eaten++;
	philo->last_meal = get_time();
	meals_counter(philo);
	pthread_mutex_unlock(philo->meal_lock);
	ft_usleep(philo->time_to_eat);
	philo->eating = 0;
	if (philo->id % 2)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->l_fork);
		pthread_mutex_unlock(philo->r_fork);
	}
}

void    *philo_routine(void *param)
{
	t_philo philo;

	philo = (*((t_philo *)param));
	while(!is_dead(&philo))
	{
		if (!check_philos_state(&philo, philo.id))
			eat(&philo);
		if (!check_philos_state(&philo, philo.id))
			sleeep(&philo);
		if (!check_philos_state(&philo, philo.id))
			think(&philo);
	}
	return (NULL);
}


int	*meals_arr(int nb_of_philo)
{
	int	*is_full;
	int	i;

	is_full = (int *)malloc(sizeof(int) * nb_of_philo);
	if (!is_full)
		return (NULL);
	i = -1;
	while (i++ < nb_of_philo)
		is_full[i] = 0;
	return (is_full);
}

int    init_program(char **av)
{
	t_observer			observer;
	t_philo 		philo[MAX_PHILO];
	pthread_mutex_t	fork[MAX_PHILO];

	if (atoi(av[1]) > MAX_PHILO)
		return (destroy_exit("Number of philosophers exceeded.\n"));
	pthread_mutex_init(&observer.dead_lock, NULL);
	pthread_mutex_init(&observer.write_lock, NULL);
	pthread_mutex_init(&observer.meal_lock, NULL);
	observer.philo = philo;
	observer.dead_flag = 0;
	observer.is_full = meals_arr(atoi(av[1]));
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
		philo[i].start_time = get_time();
		philo[i].last_meal = get_time();
		philo[i].is_full = observer.is_full;
		philo[i].dead_flag = &observer.dead_flag;
		philo[i].dead_lock = &observer.dead_lock;
		philo[i].meal_lock = &observer.meal_lock;
		philo[i].write_lock = &observer.write_lock;
		philo[i].r_fork = &fork[i];
		if (i == philo[i].nb_of_philo - 1)
			philo[i].l_fork = &fork[0];
		else
			philo[i].l_fork = &fork[i + 1];
	}
	i = -1;
	while (++i < philo[0].nb_of_philo)
	{
		if (pthread_create(&philo[i].tid, NULL, &philo_routine, &observer.philo[i]))
			return (destroy_exit("Thread creation error.\n"));
		if (philo[i].id % 2 == 0)
			ft_usleep(1);
	}
	// if (pthread_create(&observer.tid, NULL, &observer_routine, (void *)&observer.philo))
	// 	return (destroy_exit("Thread creation error.\n"));
	i = -1;
	while (++i < philo[0].nb_of_philo)
	{	
		if (pthread_join(philo[i].tid, NULL))
			return (destroy_exit("Thread joining error.\n"));
	}
	// if (pthread_join(observer.tid, NULL))
	// 		return (destroy_exit("Thread joining error.\n"));
	return (0);
}

int main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
		return (1);
	if (init_program(av))
		return (1);
}