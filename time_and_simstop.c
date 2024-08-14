/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_and_simstop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:56:39 by sruff             #+#    #+#             */
/*   Updated: 2024/08/14 22:09:12 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_stop_simulation(t_data *data)
{
	int	stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->simulation_stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
}

void	set_stop_simulation(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->simulation_stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);
}

void	print_status(t_data *data, int id, char *status)
{
	pthread_mutex_lock(&data->print_mutex);
	if (!check_stop_simulation(data))
		printf("%lld %d %s\n", get_time() - data->start_time, (id + 1), status);
	pthread_mutex_unlock(&data->print_mutex);
}
