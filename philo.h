/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:17:58 by sruff             #+#    #+#             */
/*   Updated: 2024/08/22 13:49:07 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philosopher
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				eat_count;
	long long		last_meal_time;
	pthread_mutex_t	eat_mutex;
	struct s_data	*data;
}	t_philosopher;

typedef struct s_data
{
	int				num_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	long long		start_time;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
	pthread_mutex_t	stop_mutex;
	t_philosopher	*philosophers;
	pthread_mutex_t	*forks;
	int				simulation_stop;
}	t_data;

int					ft_atoi(const char *str);
int					validate_arguments(int argc, char **argv);
long long			get_time(void);
void				precise_usleep(unsigned long usec);
int					check_stop_simulation(t_data *data);
void				set_stop_simulation(t_data *data);
void				print_status(t_data *data, int id, char *status);
void				cleanup(t_data *data, pthread_t *threads);
int					init_data(t_data *data, int argc, char **argv);
void				*monitor(void *arg);
void				*philo_lifecycle(void *arg);
int					philo_eat(t_philosopher *philo);

int					initialize_simulation(t_data *data, pthread_t **threads,
						int argc, char **argv);
int					start_monitor(t_data *data, pthread_t *monitor_thread);
void				create_philosopher_threads(t_data *data,
						pthread_t *threads);
void				join_threads(t_data *data, pthread_t *threads,
						pthread_t monitor_thread);

#endif
