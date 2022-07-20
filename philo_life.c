/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:44:19 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/07/20 22:12:49 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "philo.h"

void    monitor(long time, long action_time, t_philo *philo, const char *action)
{
    long dif;

// current time + action time  - last eaten
    dif = (get_time() + action_time) - time;
    if (dif > philo->settings->die_time)
    {
        printf("%ld #%d %s", get_time(), philo->chair, action);
        usleep(dif);
        philo->settings->dead_count++;
        printf("%ld #%d died", get_time(), philo->chair);
        exit (1);
    }
}

void    philo_eat(t_philo *philo, t_philo *neighbour)
{
    monitor(philo->last_eaten, philo->eat_time, philo, "is eating");
    printf("%ld #%d is eating\n", get_time(), philo->chair);
    philo->last_eaten = get_time();
    usleep(philo->settings->eat_time);
    pthread_mutex_unlock(&philo->fork);
    pthread_mutex_unlock(&neighbour->fork);
}
