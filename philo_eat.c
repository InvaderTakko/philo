/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 21:13:08 by sruff             #+#    #+#             */
/*   Updated: 2024/08/14 21:14:52 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_test.h"

static int	handle_single_philosopher(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(data, philo->id, "has taken a fork");
	precise_usleep(data->time_to_die * 1000);
	return (1);
}

static void	take_forks(t_philosopher *philo)
{
	t_data	*data;
	int		left_fork;
	int		right_fork;

	data = philo->data;
	left_fork = philo->id;
	right_fork = (philo->id + 1) % data->num_philosophers;
	if (left_fork < right_fork)
	{
		pthread_mutex_lock(&data->forks[left_fork]);
		pthread_mutex_lock(&data->forks[right_fork]);
	}
	else
	{
		pthread_mutex_lock(&data->forks[right_fork]);
		pthread_mutex_lock(&data->forks[left_fork]);
	}
	print_status(data, philo->id, "has taken a fork");
	print_status(data, philo->id, "has taken a fork");
}

static void	release_forks(t_philosopher *philo)
{
	t_data	*data;
	int		left_fork;
	int		right_fork;

	data = philo->data;
	left_fork = philo->id;
	right_fork = (philo->id + 1) % data->num_philosophers;
	if (left_fork < right_fork)
	{
		pthread_mutex_unlock(&data->forks[right_fork]);
		pthread_mutex_unlock(&data->forks[left_fork]);
	}
	else
	{
		pthread_mutex_unlock(&data->forks[left_fork]);
		pthread_mutex_unlock(&data->forks[right_fork]);
	}
}

static void	eat(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(data, philo->id, "is eating");
	pthread_mutex_lock(&philo->eat_mutex);
	philo->last_meal_time = get_time();
	precise_usleep(data->time_to_eat * 1000);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eat_mutex);
}

int	philo_eat(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	if (data->num_philosophers == 1)
		return (handle_single_philosopher(philo));

	take_forks(philo);
	eat(philo);
	release_forks(philo);

	return (0);
}