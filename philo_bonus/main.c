/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 22:46:45 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/11 22:47:37 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "philo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <semaphore.h>
#include <stdio.h>

int	init(int argc, char **argv, t_settings *settings)
{
	int	i;

	i = 0;
	while (++i < argc)
		if (!is_pos_int(argv[i]))
			return (0);
	settings->args = argc - 1;
	settings->philo_num = ft_atoi(argv[1]);
	settings->die_time = ft_atoi(argv[2]) * 1000;
	settings->eat_time = ft_atoi(argv[3]) * 1000;
	settings->sleep_time = ft_atoi(argv[4]) * 1000;
	settings->philos = malloc(sizeof(t_philo) * settings->philo_num);
	settings->dead_count = 0;
	settings->start_time = get_time();
	sem_unlink("/pen");
	sem_unlink("/fork_sets");
	settings->pen = sem_open("/pen", O_CREAT, 0664, 1);
	settings->fork_sets = sem_open("/fork_sets", O_CREAT, \
		0664, settings->philo_num / 2);
	if (argc == 6)
		settings->stop_after = ft_atoi(argv[5]);
	else
		settings->stop_after = -1;
	return (1);
}

void	init_eat_rounds(t_settings *settings, int i)
{
	int	eat_rounds;
	int	round;

	settings->eat_queue = malloc(sizeof(int *) * settings->philo_num + 1);
	eat_rounds = settings->philo_num;
	if (settings->philo_num % 2 == 0)
		eat_rounds = 2;
	settings->eat_rounds = eat_rounds;
	i = -1;
	while (++i < settings->philo_num)
	{
		settings->eat_queue[i] = malloc(sizeof(int) * eat_rounds + 1);
		settings->eat_queue[i][eat_rounds] = -1;
	}
	i = -1;
	while (++i < settings->philo_num)
	{
		round = -1;
		while (++round < settings->eat_rounds)
				settings->eat_queue[i][round] = 0;
	}
	if (eat_rounds == 2)
		assign_two_eat_rounds(settings);
	else
		assign_three_eat_rounds(settings, i);
}

static void	ft_error_check(int argc, char **argv, t_settings *settings)
{
	if (argc < 5 || argc > 6 || !init(argc, argv, settings))
	{
		printf("Error: Wrong arguements");
		exit (0);
	}
}

static void	init_philo(t_settings *settings, int i)
{
	settings->philos[i].chair = i;
	settings->philos[i].eat_time = settings->eat_time;
	settings->philos[i].sleep_time = settings->sleep_time;
	settings->philos[i].settings = settings;
	settings->philos[i].meals = 0;
	settings->philos[i].eat_round = 0;
	settings->philos[i].last_eaten = get_time();
	settings->philos[i].pid = fork();
}

int	main(int argc, char **argv)
{
	t_settings		settings;
	int				i;

	ft_error_check(argc, argv, &settings);
	init_eat_rounds(&settings, i);
	i = -1;
	while (++i < settings.philo_num)
	{
		init_philo(&settings, i);
		if (settings.philos[i].pid == 0)
		{
			life(&(settings.philos[i]));
			exit (1);
		}
	}
	waitpid(-1, NULL, 0);
	ft_cleanup(&settings);
	exit (1);
}
