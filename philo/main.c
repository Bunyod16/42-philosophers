/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 16:08:01 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/12 20:53:22 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
	if (argc == 6)
		settings->stop_after = ft_atoi(argv[5]);
	else
		settings->stop_after = -1;
	return (1);
}

static void	ft_error_check(int argc, char **argv, t_settings *settings)
{
	if (argc < 5 || argc > 6 || !init(argc, argv, settings))
	{
		printf("Error: Wrong arguements");
		exit (0);
	}
}

void	ft_cleanup(t_settings *settings, pthread_t *threads)
{
	int	i;

	i = -1;
	while (++i < settings->philo_num)
		free(settings->eat_queue[i]);
	i = -1;
	while (++i < settings->philo_num)
	{
		pthread_mutex_destroy(&(settings->philos[i]).roundlock);
		pthread_mutex_destroy(&(settings->philos[i]).fork);
	}
	free(settings->eat_queue);
	free(settings->philos);
	pthread_mutex_destroy(&(settings->roundlock));
	pthread_mutex_destroy(&(settings->pen));
	free(threads);
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
	pthread_mutex_init(&(settings->philos[i]).roundlock, NULL);
	pthread_mutex_init(&(settings->philos[i]).fork, NULL);
}

int	main(int argc, char **argv)
{
	t_settings	settings;
	int			i;
	pthread_t	*threads;

	i = -1;
	ft_error_check(argc, argv, &settings);
	init_eat_rounds(&settings, i);
	threads = malloc(sizeof(pthread_t) * settings.philo_num);
	pthread_mutex_init(&(settings.roundlock), NULL);
	pthread_mutex_init(&(settings.pen), NULL);
	settings.current_round = 0;
	while (++i < settings.philo_num)
	{
		init_philo(&settings, i);
		pthread_create(&threads[i], NULL, life, (void *)&settings.philos[i]);
	}
	i = -1;
	while (++i < settings.philo_num)
		pthread_join(threads[i], NULL);
	ft_cleanup(&settings, threads);
}
