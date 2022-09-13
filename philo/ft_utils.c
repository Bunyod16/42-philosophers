/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 22:48:16 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/13 11:05:37 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include "philo.h"
#include <stdlib.h>

int	ft_isnum(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = -1;
	while (str[++i])
		if (str[i] < '0' && str[i] > '9' )
			return (0);
	return (1);
}

int	ft_atoi(const char *str)
{
	int		sign;
	long	ret;

	ret = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		++str;
	if (*str == '+' || *str == '-')
		if (*(str++) == '-')
			sign *= -1;
	while (*str >= '0' && *str <= '9')
	{
		ret = ret * 10 + sign * (*str++ - '0');
		if (ret > INT_MAX)
			return (-1);
		else if (ret < INT_MIN)
			return (0);
	}
	return ((int)ret);
}

void	pen(t_philo *philo, long timestamp, const char *action, int flag)
{
	pthread_mutex_lock(&philo->settings->pen);
	printf("%ld #%d %s", timestamp - philo->settings->start_time, \
		philo->chair, action);
	if (flag == 0)
		pthread_mutex_unlock(&philo->settings->pen);
}

int	get_round(t_settings *settings, bool increase)
{
	if (increase == true)
		settings->current_round += 1;
	return (settings->current_round);
}

void	lock_forks(t_philo *p, t_philo *neighbour)
{
	pthread_mutex_lock(&neighbour->fork);
	pen(p, get_time(), "has taken fork\n", 0);
	pthread_mutex_lock(&p->fork);
	pen(p, get_time(), "has taken fork\n", 0);
}
