/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 21:13:08 by sruff             #+#    #+#             */
/*   Updated: 2024/08/22 16:27:21 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	handle_single_philosopher(t_philosopher *philo)
{
	print_status(philo->data, philo->id, "has taken a fork");
	precise_usleep(philo->data->time_to_die * 1000);
	print_status(philo->data, philo->id, "died");
	set_stop_simulation(philo->data);
	return (1);
}

static void	take_forks(t_philosopher *philo)
{
	int		left_fork;
	int		right_fork;

	left_fork = philo->id;
	right_fork = philo->right_fork;
	if (left_fork < right_fork)
	{
		pthread_mutex_lock(&philo->data->forks[left_fork]);
		pthread_mutex_lock(&philo->data->forks[right_fork]);
	}
	else
	{
		pthread_mutex_lock(&philo->data->forks[right_fork]);
		pthread_mutex_lock(&philo->data->forks[left_fork]);
	}
	print_status(philo->data, philo->id, "has taken a fork");
	print_status(philo->data, philo->id, "has taken a fork");
}

static void	release_forks(t_philosopher *philo)
{
	int		left_fork;
	int		right_fork;

	left_fork = philo->id;
	right_fork = philo->right_fork;
	if (left_fork < right_fork)
	{
		pthread_mutex_unlock(&philo->data->forks[right_fork]);
		pthread_mutex_unlock(&philo->data->forks[left_fork]);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->forks[left_fork]);
		pthread_mutex_unlock(&philo->data->forks[right_fork]);
	}
}

static void	eat(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->eat_mutex);
	print_status(philo->data, philo->id, "is eating");
	philo->last_meal_time = get_time();
	precise_usleep(philo->data->time_to_eat * 1000);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eat_mutex);
}

int	philo_eat(t_philosopher *philo)
{
	if (philo->data->num_philosophers == 1)
		return (handle_single_philosopher(philo));
	take_forks(philo);
	eat(philo);
	release_forks(philo);
	return (0);
}
