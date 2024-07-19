/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:51:44 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/19 16:53:14 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

int	print_error(char *err)
{
	write(2, err, ft_strlen(err));
	return (1);
}

static int	treat_first_args(t_args *args, char **av)
{
	if (args->nb_of_philo > MAX_PHILO)
		return (print_error("Number of philosophers exceeded.\n"),
			free(args), 1);
	args->time_to_die = ft_atoi(av[2]);
	args->time_to_eat = ft_atoi(av[3]);
	args->time_to_sleep = ft_atoi(av[4]);
	if (args->nb_of_philo < 0 || args->time_to_die < 60
		|| args->time_to_eat < 60 || args->time_to_sleep < 60)
		return (print_error("Only positive numbers are allowed.\n Minimum is 60 (N >= 60).\nExcept for nb_of_philo (N > 0).\n"),
			free(args), 1);
	return (0);
}

t_args	*parse_args(char **av)
{
	t_args	*args;

	args = (t_args *)malloc(sizeof(t_args));
	if (!args)
		return (NULL);
	args->nb_of_philo = ft_atoi(av[1]);
	if (treat_first_args(args, av))
		return (NULL);
	if (av[5])
	{
		args->av = av[5];
		args->meals_to_eat = ft_atoi(av[5]);
		if (args->meals_to_eat <= 0)
			return (print_error("Only positive numbers are allowed (N > 0).\n"),
				free(args), NULL);
	}
	else
	{
		args->av = NULL;
		args->meals_to_eat = -1;
	}
	return (args);
}
