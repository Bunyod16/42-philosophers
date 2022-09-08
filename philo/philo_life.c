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

void    wait_all_philo_eat(t_philo *philo)
{
    int i;
    int global_round;

    i = 0;
    pthread_mutex_lock(&(philo->settings->roundlock));
    global_round = get_round(philo->settings, false);
    pthread_mutex_unlock(&(philo->settings->roundlock));
    while (i < philo->settings->philo_num)
    {
        pthread_mutex_lock(&(philo->settings->philos[i].roundlock));
        if (philo->settings->philos[i].eat_round > global_round)
        {
            pthread_mutex_unlock(&(philo->settings->philos[i].roundlock));
            i++;
        }
        else
            pthread_mutex_unlock(&(philo->settings->philos[i].roundlock));
    }
}

void    philo_eat(t_philo *philo, t_philo *neighbour, int round)
{
    int i;

    philo->last_eaten = get_time();
    monitor(philo->last_eaten, philo->eat_time / 1000, philo, "is eating\n");
    pen(philo->settings, get_time(), philo->chair, "is eating\n", 0);
    ++philo->meals;
    usleep(philo->settings->eat_time);
    pthread_mutex_unlock(&philo->fork);
    pthread_mutex_unlock(&neighbour->fork);
    monitor(philo->last_eaten, philo->sleep_time / 1000, philo, "is sleeping\n");
	pen(philo->settings, get_time(), philo->chair, "is sleeping\n", 0);
    i = philo->settings->philo_num - 1;
    while (philo->settings->eat_queue[i][round % philo->settings->eat_rounds] != 1)
        --i;
    pthread_mutex_lock(&(philo->roundlock));
    philo->eat_round++;
    pthread_mutex_unlock(&(philo->roundlock));
    if (philo->chair == i)
    {
        wait_all_philo_eat(philo);
    	pthread_mutex_lock(&(philo->settings->roundlock));
        get_round(philo->settings, true);
    	pthread_mutex_unlock(&(philo->settings->roundlock));
    }
}

void    *life(void *philo_arg)
{
	t_philo     *philo;
	t_philo     *neighbour;
	int         i;

	philo = (t_philo *)philo_arg;
	i = philo->chair;
	if (philo->chair == philo->settings->philo_num - 1)
		i = philo->settings->philo_num;
	neighbour = &(philo->settings->philos[i + 1]);
	monitor(philo->last_eaten, time_till_eat(philo, 0, false), philo, NULL);
	while (philo->settings->stop_after == -1 || philo->settings->stop_after > philo->meals)
	{
		if (philo->settings->eat_queue[philo->chair][philo->eat_round % philo->settings->eat_rounds] == 1)
		{
			pthread_mutex_lock(&neighbour->fork);
			pen(philo->settings, get_time(), philo->chair, "has taken fork\n", 0);
			pthread_mutex_lock(&philo->fork);
			pen(philo->settings, get_time(), philo->chair, "has taken fork\n", 0);
			philo_eat(philo, neighbour, philo->eat_round);
			usleep(philo->settings->sleep_time);
		}
		else
        {
            pthread_mutex_lock(&(philo->roundlock));
            philo->eat_round++;
            pthread_mutex_unlock(&(philo->roundlock));
            if (philo->eat_round - 1 == 0 
			|| (philo->eat_round - 1 % philo->settings->eat_rounds > 0 
				&& philo->settings->eat_queue[philo->chair][(philo->eat_round - 1 % philo->settings->eat_rounds) - 1] != 0))
		    {
			    monitor(philo->last_eaten, time_till_eat(philo, philo->eat_round - 1, false) - (philo->sleep_time / 1000), philo, "is thinking\n");//check without deducting time to sleep on the first round
			    pen(philo->settings, get_time(), philo->chair, "is thinking\n", 0);
            }
        }
		while (usleep(10) == 0)
        {
            pthread_mutex_lock(&(philo->settings->roundlock));
            if (get_round(philo->settings, false) >= philo->eat_round )
                break;
            pthread_mutex_unlock(&(philo->settings->roundlock));
        }
        pthread_mutex_unlock(&(philo->settings->roundlock));
		pen(philo->settings, get_time(), philo->chair, "is OUT\n", 0);
	}
    pthread_mutex_lock(&(philo->roundlock));
    philo->eat_round = 999999;
    pthread_mutex_unlock(&(philo->roundlock));
	return (NULL);
}
