/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_eat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 02:27:49 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/11 22:25:16 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

void	assign_three_eat_rounds(t_settings *settings, int i)
{
	int	*fork_sets;
	int	eater_num;
	int	round;

	fork_sets = malloc(sizeof(int) * ((settings->philo_num / 2)));
	eater_num = 0;
	i = -1;
	while (++i < settings->philo_num / 2)
	{
		fork_sets[i] = eater_num;
		eater_num += 2;
	}
	round = -1;
	while (++round < settings->eat_rounds)
	{
		i = -1;
		while (++i < settings->philo_num / 2)
		{
			settings->eat_queue[fork_sets[i]][round] = 1;
			fork_sets[i] += 1;
			if (fork_sets[i] == settings->philo_num)
				fork_sets[i] = 0;
		}
	}
	free(fork_sets);
}

void	assign_two_eat_rounds(t_settings *settings)
{
	int	n_philo;
	int	round;

	n_philo = -1;
	while (++n_philo < settings->philo_num)
	{
		round = -1;
		while (++round < settings->eat_rounds)
		{
			if (n_philo % 2 == 0 && round % 2 == 0)
				settings->eat_queue[n_philo][round] = 1;
			else if (n_philo % 2 != 0 && round % 2 != 0)
				settings->eat_queue[n_philo][round] = 1;
			else
				settings->eat_queue[n_philo][round] = 0;
		}
	}
}
