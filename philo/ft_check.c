/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 16:45:53 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/13 10:53:51 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <limits.h>

int	is_pos_int(char *str)
{
	if (!str)
		return (0);
	while (*str != '\0')
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

long int	get_time(void)
{
	struct timeval	current_time;
	long int		ret;

	gettimeofday(&current_time, NULL);
	ret = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (ret);
}

void	wait_all_philo_eat(t_philo *philo)
{
	int	i;
	int	global_round;

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

int	ate_last_round(t_philo *philo)
{
	return (philo->settings->eat_queue[philo->chair] \
		[(philo->eat_round - 1) % philo->settings->eat_rounds]);
}

long	next_eat(t_philo *philo)
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
