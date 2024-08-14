/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:18:03 by sruff             #+#    #+#             */
/*   Updated: 2024/08/14 21:34:55 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_philosopher_status(t_data *data, int i, long long time)
{
	pthread_mutex_lock(&data->philosophers[i].eat_mutex);
	if (time - data->philosophers[i].last_meal_time > data->time_to_die)
	{
		print_status(data, i, "died");
		set_stop_simulation(data);
		pthread_mutex_unlock(&data->philosophers[i].eat_mutex);
		return (1);
	}
	if (data->must_eat_count > 0
		&& data->philosophers[i].eat_count >= data->must_eat_count)
	{
		pthread_mutex_unlock(&data->philosophers[i].eat_mutex);
		return (2);
	}
	pthread_mutex_unlock(&data->philosophers[i].eat_mutex);
	return (0);
}

static int	check_all_philosophers(t_data *data)
{
	int			i;
	int			finished_count;
	long long	time;
	int			status;

	i = 0;
	finished_count = 0;
	while (i < data->num_philosophers)
	{
		time = get_time();
		status = check_philosopher_status(data, i, time);
		if (status == 1)
			return (1);
		if (status == 2)
			finished_count++;
		i++;
	}
	return (finished_count == data->num_philosophers);
}

void	*monitor(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_all_philosophers(data))
		{
			set_stop_simulation(data);
			return (NULL);
		}
		precise_usleep(1000);
	}
	return (NULL);
}

void	*philo_lifecycle(void *arg)
{
	t_philosopher	*philo;
	t_data			*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	if (philo->id % 2 != 0)
		precise_usleep(1000);
	while (!check_stop_simulation(data))
	{
		if (philo_eat(philo) != 0)
			break ;
		print_status(data, philo->id, "is sleeping");
		precise_usleep(data->time_to_sleep * 1000);
		print_status(data, philo->id, "is thinking");
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	*threads;
	pthread_t	monitor_thread;

	if (argc != 5 && argc != 6)
	{
		printf("Usage:%s number_of_philosophers time_to_die time_to_eat time_to_sleep[number_of_times_each_philosopher_must_eat]\n ",
			argv[0]);
		return (1);
	}
	if (!initialize_simulation(&data, &threads, argc, argv))
		return (1);
	if (!start_monitor(&data, &monitor_thread))
		return (1);
	create_philosopher_threads(&data, threads);
	join_threads(&data, threads, monitor_thread);
	cleanup(&data, threads);
	return (0);
}