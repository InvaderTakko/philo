/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_join_threads.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:49:38 by sruff             #+#    #+#             */
/*   Updated: 2024/08/14 22:10:25 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	initialize_simulation(t_data *data, pthread_t **threads, int argc,
	char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) < 0)
		{
			printf("Error: Invalid rgument '%s'.\n", argv[i]);
			return (0);
		}
		i++;
	}
	if (!init_data(data, argc, argv))
	{
		printf("Error initializing data\n");
		return (0);
	}
	*threads = malloc(sizeof(pthread_t) * data->num_philosophers);
	if (!*threads)
	{
		cleanup(data, NULL);
		return (0);
	}
	return (1);
}

int	start_monitor(t_data *data, pthread_t *monitor_thread)
{
	if (pthread_create(monitor_thread, NULL, monitor, data) != 0)
	{
		cleanup(data, NULL);
		return (0);
	}
	return (1);
}

void	create_philosopher_threads(t_data *data, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers && !check_stop_simulation(data))
	{
		if (pthread_create(&threads[i], NULL, philo_lifecycle,
				&data->philosophers[i]) != 0)
			set_stop_simulation(data);
		i++;
	}
}

void	join_threads(t_data *data, pthread_t *threads, pthread_t monitor_thread)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
}
