/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:17:57 by sruff             #+#    #+#             */
/*   Updated: 2024/08/22 16:27:34 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_valid_integer(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	int		plusminus;
	long	number;

	plusminus = 1;
	number = 0;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		++str;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			plusminus *= -1;
		str++;
	}
	if (plusminus == -1)
		return (-1);
	while (*str >= 48 && *str <= 57)
	{
		number = number * 10 + (*str - '0');
		if (plusminus == 1 && number > INT_MAX)
			return (-1);
		str++;
	}
	if (*str != '\0')
		return (-1);
	return (number * plusminus);
}

int	validate_arguments(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (!is_valid_integer(argv[i]) || ft_atoi(argv[i]) <= 0)
		{
			printf("Error: Argument %d is not a valid integer.\n", i);
			return (0);
		}
		i++;
	}
	return (1);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_usleep(unsigned long usec)
{
	unsigned long	elapsed;
	struct timeval	start;
	struct timeval	now;

	elapsed = 0;
	gettimeofday(&start, NULL);
	while (elapsed < usec)
	{
		usleep(250);
		gettimeofday(&now, NULL);
		elapsed = (now.tv_sec - start.tv_sec) * 1000000UL + (now.tv_usec
				- start.tv_usec);
	}
}
