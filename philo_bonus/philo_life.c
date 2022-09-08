/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:44:19 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/06 20:20:13 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "philo.h"
#include <stdbool.h>
#include <limits.h>

static long time_till_eat(t_philo *philo, int round, bool sleep)
{
	int i;

	i = 0;
	if (philo->settings->philo_num == 1)
		return (LONG_MAX);
	while (philo->settings->eat_queue[philo->chair][round % philo->settings->eat_rounds] != 1)
	{
		round++;
		i++;
	}
    if (sleep == true)
        return ((philo->sleep_time) + (i * philo->eat_time / 1000));
	return (i * (philo->eat_time / 1000));
}

void    monitor(long last_eat_time, long action_time, t_philo *philo, const char *action)
{
    long die_at;

    die_at = last_eat_time + (philo->settings->die_time / 1000);
    if (get_time() + action_time > die_at || action_time == LONG_MAX)
    {
        if (action != NULL)
            pen(philo->settings, get_time(), philo->chair, action, 0);
        if (action_time != LONG_MAX)
            usleep((die_at - get_time()) * 1000);
        pen(philo->settings, get_time(), philo->chair, "died\n", 1);
        exit (3);
    }
}

void    philo_eat(t_philo *philo)
{
    philo->last_eaten = get_time();
    monitor(philo->last_eaten, philo->eat_time / 1000, philo, "is eating\n");
    pen(philo->settings, get_time(), philo->chair, "is eating\n", 0);
    ++philo->meals;
    usleep(philo->settings->eat_time);
    sem_post(&philo->settings->fork_sets);
    monitor(philo->last_eaten, philo->sleep_time / 1000, philo, "is sleeping\n");
	pen(philo->settings, get_time(), philo->chair, "is sleeping\n", 0);
    philo->eat_round++;
}

void    *life(t_philo *philo)
{
	int         i;

	i = philo->chair;
	if (philo->chair == philo->settings->philo_num - 1)
		i = philo->settings->philo_num;
	monitor(philo->last_eaten, time_till_eat(philo, 0, false), philo, NULL);
	while (philo->settings->stop_after == -1 || philo->settings->stop_after > philo->meals)
	{
		if (philo->settings->eat_queue[philo->chair][philo->eat_round % philo->settings->eat_rounds] == 1)
		{
            sem_wait(&philo->settings->fork_sets);
			pen(philo->settings, get_time(), philo->chair, "has taken fork\n", 0);
			pen(philo->settings, get_time(), philo->chair, "has taken fork\n", 0);
			philo_eat(philo);
			usleep(philo->settings->sleep_time);
		}
		else
        {
            philo->eat_round++;
            if (philo->eat_round - 1 == 0 
			|| (philo->eat_round - 1 % philo->settings->eat_rounds > 0 
				&& philo->settings->eat_queue[philo->chair][(philo->eat_round - 1 % philo->settings->eat_rounds) - 1] != 0))
		    {
			    monitor(philo->last_eaten, time_till_eat(philo, philo->eat_round - 1, false) - (philo->sleep_time / 1000), philo, "is thinking\n");//check without deducting time to sleep on the first round
			    pen(philo->settings, get_time(), philo->chair, "is thinking\n", 0);
                usleep(philo->eat_time + philo->sleep_time);
            }
        }
	}
	return (NULL);
}
