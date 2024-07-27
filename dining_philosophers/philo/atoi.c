/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msaadidi <msaadidi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:44:36 by msaadidi          #+#    #+#             */
/*   Updated: 2024/07/27 18:36:09 by msaadidi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static int	ft_is_space(int c)
{
	return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\r');
}

static int	skip_white_spaces(char *str, int i)
{
	while (ft_is_space(str[i]))
		i++;
	return (i);
}

static int	spaces_left(char *str, int i)
{
	while (str[i])
	{
		if (ft_is_space(str[i]))
			i++;
		else
			return (1);
	}
	return (0);
}

int	ft_atoi(char *str)
{
	int		i;
	long	result;
	int		sign;

	i = 0;
	result = 0;
	sign = 1;
	i = skip_white_spaces(str, i);
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			result = result * 10 + str[i] - '0';
		else if (spaces_left(str, i))
			return (-1);
		if (!(((result * sign) <= INT_MAX && (result * sign) >= INT_MIN)))
			return (-1);
		i++;
	}
	return (result * sign);
}
