/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:44:19 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/13 15:15:15 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "philo.h"
#include <stdbool.h>
#include <limits.h>

void	philo_eat(t_philo *philo, t_philo *neighbour, int round)
{
	int	i;

	philo->last_eaten = get_time();
	monitor(philo->eat_time / 1000, philo, "is eating\n");
	pen(philo, get_time(), "is eating\n", 0);
	++philo->meals;
	usleep(philo->settings->eat_time);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&neighbour->fork);
	pthread_mutex_lock(&(philo->roundlock));
	philo->eat_round++;
	pthread_mutex_unlock(&(philo->roundlock));
	i = philo->settings->philo_num - 1;
	while (philo->settings->eat_queue[i]
		[round % philo->settings->eat_rounds] != 1)
		--i;
	if (philo->chair == i)
	{
		wait_all_philo_eat(philo);
		pthread_mutex_lock(&(philo->settings->roundlock));
		get_round(philo->settings, true);
		pthread_mutex_unlock(&(philo->settings->roundlock));
	}
}

void	wait_round_end(t_philo *p)
{
	while (usleep(10) == 0)
	{
		pthread_mutex_lock(&(p->settings->roundlock));
		if (get_round(p->settings, false) >= p->eat_round)
			break ;
		pthread_mutex_unlock(&(p->settings->roundlock));
	}
	pthread_mutex_unlock(&(p->settings->roundlock));
}

void	philo_think(t_philo *p)
{
	if (ate_last_round(p))
	{
		monitor(next_eat(p) - p->sleep_time / 1000, p, "is thinking\n");
		pen(p, get_time(), "is thinking\n", 0);
	}
	else if (p->eat_round == 0)
	{
		monitor(next_eat(p), p, "is thinking\n");
		pen(p, get_time(), "is thinking\n", 0);
	}
}

void	start_philo(t_philo *p, t_philo *neighbour)
{
	while (p->settings->stop_after == -1 || p->settings->stop_after > p->meals)
	{
		if (p->settings->eat_queue[p->chair] \
			[p->eat_round % p->settings->eat_rounds] == 1)
		{
			lock_forks(p, neighbour);
			philo_eat(p, neighbour, p->eat_round);
			monitor(p->sleep_time / 1000, p, "is sleeping\n");
			pen(p, get_time(), "is sleeping\n", 0);
			usleep(p->settings->sleep_time);
		}
		else
		{
			philo_think(p);
			p->eat_round++;
		}
		wait_round_end(p);
	}
}

void	*life(void *philo_arg)
{
	t_philo		*p;
	t_philo		*neighbour;
	int			i;

	p = (t_philo *)philo_arg;
	i = p->chair;
	if (p->chair == p->settings->philo_num - 1)
		i = p->settings->philo_num;
	neighbour = &(p->settings->philos[i + 1]);
	monitor(next_eat(p), p, NULL);
	start_philo(p, neighbour);
	pthread_mutex_lock(&(p->roundlock));
	p->eat_round = 999999;
	pthread_mutex_unlock(&(p->roundlock));
	return (NULL);
}
