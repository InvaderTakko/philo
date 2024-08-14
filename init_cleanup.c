/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:21:15 by sruff             #+#    #+#             */
/*   Updated: 2024/08/14 21:07:24 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_test.h"

void	cleanup(t_data *data, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philosophers[i].eat_mutex);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->data_mutex);
	free(data->philosophers);
	free(data->forks);
	free(threads);
}

static int	mutex_init(t_data *data)
{
	if (!data->forks)
	{
		free(data->philosophers);
		return (0);
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0
		|| pthread_mutex_init(&data->stop_mutex, NULL) != 0
		|| pthread_mutex_init(&data->data_mutex, NULL) != 0)
	{
		free(data->philosophers);
		free(data->forks);
		return (0);
	}
	return (1);
}

static int	init_philo(t_data *data)
{
	int	i;

	i = 0;
	data->start_time = get_time();
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0
			|| pthread_mutex_init(&data->philosophers[i].eat_mutex, NULL) != 0)
		{
			cleanup(data, NULL);
			return (0);
		}
		data->philosophers[i].id = i;
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = (i + 1) % data->num_philosophers;
		data->philosophers[i].eat_count = 0;
		data->philosophers[i].last_meal_time = data->start_time;
		data->philosophers[i].data = data;
		i++;
	}
	return (1);
}

int	init_data(t_data *data, int argc, char **argv)
{
	int	i;

	data->num_philosophers = ft_atoi(argv[1]);
	data->philosophers = malloc(sizeof(t_philosopher) * data->num_philosophers);
	if (!data->philosophers)
		return (0);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat_count = -1;
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	data->simulation_stop = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philosophers);
	if (mutex_init(data) == 0)
		return (0);
	if (init_philo(data) == 0)
		return (0);
	return (1);
}
