/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:03:38 by msaadidi          #+#    #+#             */
/*   Updated: 2024/06/14 16:03:16 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
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

void	print_msg(t_philo *philo, char *act, char *color)
{
	if (!is_dead(philo))
		printf("%s%ld %d %s\n", color, get_time() - philo->start_time,
			philo->id, act);
}

int	*meals_arr(int nb_of_philo)
{
	int	*is_full;
	int	i;

	is_full = (int *)malloc(sizeof(int) * nb_of_philo);
	if (!is_full)
		return (NULL);
	i = -1;
	while (++i < nb_of_philo)
		is_full[i] = 0;
	return (is_full);
}
