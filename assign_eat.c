/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_eat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 02:27:49 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/08/29 02:29:01 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

void	assign_three_eat_rounds(t_settings *settings)
{
	int i;
	int *fork_sets;
	int	eater_num;
	int round;

	fork_sets = malloc(sizeof(int) * ((settings->philo_num / 2)));
	i = 0;
	eater_num = 0;
	while (i < settings->philo_num / 2)
	{
		fork_sets[i++] = eater_num;
		eater_num += 2;
	}
	round = 0;
	while (round < settings->eat_rounds) // for every philo
	{
		i = 0;
		while (i < settings->philo_num / 2) // run for the amount of eaters
		{
			settings->eat_queue[fork_sets[i]][round] = 1; // lets this philo eat
			fork_sets[i] += 1; // number of the philo that should eat next round
			if (fork_sets[i] == settings->philo_num)
				fork_sets[i] = 0; // if philo num exceeds max philo, return to 1st philo
			i++;
		}
		round += 1;
	}
}

void	assign_two_eat_rounds(t_settings *settings)
{
	int n_philo;
	int round;

	n_philo = -1;
	while (++n_philo < settings->philo_num) // for every philo
	{
		round = -1;
		while (++round < settings->eat_rounds) // for every round
		{
			if (n_philo % 2 == 0 && round % 2 == 0) // even philo and even round
				settings->eat_queue[n_philo][round] = 1;
			else if (n_philo % 2 != 0 && round % 2 != 0) // odd philo and odd round
				settings->eat_queue[n_philo][round] = 1;
			else
				settings->eat_queue[n_philo][round] = 0;
		}
	}
}