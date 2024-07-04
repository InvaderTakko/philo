/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:17:58 by sruff             #+#    #+#             */
/*   Updated: 2024/07/04 23:59:53 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philosopher {
	int id;
	int left_fork;
	int right_fork;
	int eat_count;
	long long last_meal_time;
	struct s_data *data;
} t_philosopher;

typedef struct s_data {
	int	num_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_count;
	long long	start_time;
	pthread_mutex_t	print_mutex;
	// pthread_mutex_t	meal_mutex; //prob stupid idea
	t_philosopher	*philosophers;
	//t_philosopher philosophers[200]; ???? either really smart or stupid
	pthread_mutex_t	*forks;
	int	simulation_stop;
} t_data;

#endif