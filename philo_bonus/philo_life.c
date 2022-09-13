/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 18:44:19 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/13 15:22:59 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "philo.h"
#include <stdbool.h>
#include <limits.h>

static long	next_eat(t_philo *philo)
{
	int	i;
	int	round;

	round = philo->eat_round;
	i = 0;
	if (philo->settings->philo_num == 1)
		return (LONG_MAX);
	while (philo->settings->eat_queue[philo->chair] \
		[round % philo->settings->eat_rounds] != 1)
	{
		i++;
		round++;
	}
	return (i * (philo->eat_time / 1000));
}

void	philo_eat(t_philo *philo)
{
	philo->last_eaten = get_time();
	monitor(philo->eat_time / 1000, philo, "is eating\n");
	pen(philo, get_time(), "is eating\n", 0);
	++philo->meals;
	usleep(philo->settings->eat_time);
	sem_post(philo->settings->fork_sets);
	monitor(philo->sleep_time / 1000, philo, "is sleeping\n");
	pen(philo, get_time(), "is sleeping\n", 0);
	philo->eat_round++;
}

int	ate_last_round(t_philo *philo)
{
	return (philo->settings->eat_queue[philo->chair] \
		[(philo->eat_round - 1) % philo->settings->eat_rounds]);
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

void	life(t_philo *p)
{
	monitor(next_eat(p), p, NULL);
	while (p->settings->stop_after == -1 || p->settings->stop_after > p->meals)
	{
		if (p->settings->eat_queue[p->chair] \
			[p->eat_round % p->settings->eat_rounds] == 1)
		{
			sem_wait(p->settings->fork_sets);
			pen(p, get_time(), "has taken fork\n", 0);
			pen(p, get_time(), "has taken fork\n", 0);
			philo_eat(p);
			usleep(p->settings->sleep_time);
		}
		else
		{
			philo_think(p);
			p->eat_round++;
		}
		usleep(next_eat(p));
	}
	while (p->chair != p->settings->philo_num - 1)
		usleep(1000);
}
