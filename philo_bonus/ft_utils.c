/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/20 22:48:16 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/11 22:44:15 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include "philo.h"
#include <stdlib.h>
#include <signal.h>

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

long int	get_time(void)
{
	struct timeval	current_time;
	long int		ret;

	gettimeofday(&current_time, NULL);
	ret = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (ret);
}

void	pen(t_philo *philo, long timestamp, const char *action, int flag)
{
	sem_wait(philo->settings->pen);
	printf("%ld #%d %s", timestamp - philo->settings->start_time, \
		philo->chair, action);
	if (flag == 0)
		sem_post(philo->settings->pen);
}

void	ft_cleanup(t_settings *settings)
{
	int	i;

	i = -1;
	while (++i < settings->philo_num)
		free(settings->eat_queue[i]);
	free(settings->eat_queue);
	free(settings->philos);
	sem_unlink("/fork_sets");
	sem_unlink("/pen");
	i = -1;
	while (++i < settings->philo_num)
		kill(settings->philos[i].pid, SIGTERM);
}
