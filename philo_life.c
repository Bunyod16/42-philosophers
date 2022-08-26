/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:44:19 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/08/25 14:44:51 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "philo.h"
#include <stdbool.h>

void    monitor(long time, long action_time, t_philo *philo, const char *action)
{
    long dif;

// current time + action time  - last eaten
    dif = (get_time() + action_time) - time;
    if (dif > philo->settings->die_time)
    {
        if (action != NULL)
            pen(philo->settings, get_time(), philo->chair, action, 0);
        usleep(dif);
        pen(philo->settings, get_time(), philo->chair, "died", 1);
        exit (1);
    }
}

void    philo_eat(t_philo *philo, t_philo *neighbour, int round)
{
    int i;

    philo->last_eaten = get_time();
    monitor(philo->last_eaten, philo->eat_time, philo, "is eating\n");
    pen(philo->settings, get_time(), philo->chair, "is eating\n", 0);
    usleep(philo->settings->eat_time);
    pthread_mutex_unlock(&philo->fork);
    pthread_mutex_unlock(&neighbour->fork);
    // if this is the last philo that should eat in this round, increment the eat round
    monitor(philo->last_eaten, philo->sleep_time, philo, "is sleeping\n");
	pen(philo->settings, get_time(), philo->chair, "is sleeping\n", 0);
    i = philo->settings->philo_num - 1;
    while (philo->settings->eat_queue[i][round] != 1)
        --i;
    if (philo->chair == i)
    {
    	pthread_mutex_lock(&(philo->settings->roundlock));
        get_round(philo->settings, true);
    	pthread_mutex_unlock(&(philo->settings->roundlock));
    }
}
