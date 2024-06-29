/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 15:18:03 by sruff             #+#    #+#             */
/*   Updated: 2024/06/29 18:27:39 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// code from https://code-vault.net/lesson/18ec1942c2da46840693efe9b51eabf6
// testing simulatanious acces with/without locking

int number_go_brrr = 0;
pthread_mutex_t mutex;

void	*brrr()
{
	int	i;

	i = 0;
    while(i < 10000000)
	{
        pthread_mutex_lock(&mutex);
        number_go_brrr++;
        pthread_mutex_unlock(&mutex);
		i++;
    }
	return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2, p3, p4;
    pthread_mutex_init(&mutex, NULL);
    if (pthread_create(&p1, NULL, &brrr, NULL) != 0)
        return 1;
    if (pthread_create(&p2, NULL, &brrr, NULL) != 0)
        return 2;
    if (pthread_create(&p3, NULL, &brrr, NULL) != 0)
        return 3;
    if (pthread_create(&p4, NULL, &brrr, NULL) != 0)
        return 4;
    if (pthread_join(p1, NULL) != 0)
        return 5;
    if (pthread_join(p2, NULL) != 0)
        return 6;
    if (pthread_join(p3, NULL) != 0)
        return 7;
    if (pthread_join(p4, NULL) != 0)
        return 8;
    pthread_mutex_destroy(&mutex);
    printf("Number go brrr %d\n", number_go_brrr);
    return 0;
}