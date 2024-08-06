/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:18:03 by sruff             #+#    #+#             */
/*   Updated: 2024/08/06 18:51:36 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

static void	precise_usleep(unsigned long usec)
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
	free(data->philosophers);
	free(data->forks);
	free(threads);
}

static int	ft_atoi(const char *str)
{
	int	plusminus;
	int	number;

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

static long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static void	print_status(t_data *data, int id, char *status)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!data->simulation_stop)
		printf("%lld %d %s\n", get_time() - data->start_time, (id + 1), status);
	pthread_mutex_unlock(&data->print_mutex);
}

static int	check_stop_simulation(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->simulation_stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
}

static void	set_stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->simulation_stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);
}

static void	*monitor(void *arg)
{
	t_data		*data;
	int			i;
	int			all_finished;
	long long	time;

	data = (t_data *)arg;
	// time = 0;
	while (1)
	{
		i = 0;
		all_finished = 1;
		while (i < data->num_philosophers)
		{
			time = get_time();
			pthread_mutex_lock(&data->philosophers[i].eat_mutex);
			if (time - data->philosophers[i].last_meal_time > data->time_to_die)
			{
				print_status(data, i, "died");
				set_stop_simulation(data);
				pthread_mutex_unlock(&data->philosophers[i].eat_mutex);
				return (NULL);
			}
			if (data->must_eat_count > 0
				&& data->philosophers[i].eat_count < data->must_eat_count)
				all_finished = 0;
			pthread_mutex_unlock(&data->philosophers[i].eat_mutex);
			i++;
		}
		if (data->must_eat_count > 0 && all_finished)
		{
			// usleep(data->time_to_eat * 1000);
			set_stop_simulation(data);
			// Allow time for the last philosopher to finish eating
			return (NULL);
		}
		precise_usleep(250);
	}
	return (NULL);
}

static int	philo_eat(t_philosopher *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&data->forks[philo->left_fork]);
	print_status(data, philo->id, "has taken a fork");
	if (data->num_philosophers == 1)
	{
		precise_usleep(data->time_to_die * 1000);
		pthread_mutex_unlock(&data->forks[philo->left_fork]);
		return (1);
	}
	pthread_mutex_lock(&data->forks[philo->right_fork]);
	print_status(data, philo->id, "has taken a fork");
	print_status(data, philo->id, "is eating");
	pthread_mutex_lock(&philo->eat_mutex);
	philo->last_meal_time = get_time();
	precise_usleep(data->time_to_eat * 1000);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eat_mutex);
	// print_status(data, philo->id, "finished eating");
	pthread_mutex_unlock(&data->forks[philo->right_fork]);
	pthread_mutex_unlock(&data->forks[philo->left_fork]);
	// print_status(data, philo->id, "is sleeping");
	// precise_usleep(data->time_to_sleep * 1000);
	// print_status(data, philo->id, "is thinking");
	return (0);
}

static void	*philo_lifecycle(void *arg)
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
		// if (data->must_eat_count > 0
		//	&& philo->eat_count >= data->must_eat_count)
		// {
		// 	// print_status(data, philo->id, "is sleeping");
		// 	// precise_usleep(data->time_to_sleep * 1000);
		// 	// print_status(data, philo->id, "is thinking");
		//     break ;
		// }
		print_status(data, philo->id, "is sleeping");
		precise_usleep(data->time_to_sleep * 1000);
		print_status(data, philo->id, "is thinking");
		// precise_usleep(1000); // Small delay to prevent CPU hogging
	}
	return (NULL);
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
	if (!data->philosophers || !data->forks)
		return (0);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->stop_mutex, NULL);
	data->start_time = get_time();
	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			// init mutex first before pthread_create
			return (0);
		if (pthread_mutex_init(&data->philosophers[i].eat_mutex, NULL) != 0)
			// init mutex first before pthread_create
			return (0);
		data->philosophers[i].id = i;
		data->philosophers[i].left_fork = i;
		data->philosophers[i].right_fork = (i + 1) % data->num_philosophers;
		data->philosophers[i].eat_count = 0;
		data->philosophers[i].last_meal_time = data->start_time;
			// or get_time();
		data->philosophers[i].data = data;
		i++;
	}
	// if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	// 	return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data		data;
	pthread_t	*threads;
	pthread_t	monitor_thread;
	int			i;

	if (argc != 5 && argc != 6)
	{
		printf("Usage:%s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n",
			argv[0]);
		return (1);
	}
	if (!init_data(&data, argc, argv))
	{
		printf("Error initializing data\n");
		return (1);
	}
	threads = malloc(sizeof(pthread_t) * data.num_philosophers);
	if (!threads)
		return (1);
	i = 0;
	pthread_create(&monitor_thread, NULL, monitor, &data);
	while (i < data.num_philosophers)
	{
		pthread_create(&threads[i], NULL, philo_lifecycle,
			&data.philosophers[i]);
		i++;
	}
	i = 0;
	while (i < data.num_philosophers)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	cleanup(&data, threads);
	// pthread_mutex_destroy(&data->print_mutex);
	// free(data->philosophers);
	// free(data->forks);
	// free(threads);
	return (0);
}