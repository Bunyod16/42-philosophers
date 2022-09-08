#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

int init(int argc, char **argv, t_settings *settings)
{
	int i;

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

void init_eat_rounds(t_settings *settings)
{
	int	eat_rounds;
	int	i;
	int round;

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
	while (++i < settings->philo_num) // for every philo
	{
		round = -1;
		while (++round < settings->eat_rounds) // for every round
				settings->eat_queue[i][round] = 0;
	}
	if (eat_rounds == 2)
		assign_two_eat_rounds(settings);
	else
		assign_three_eat_rounds(settings);
}

void ft_cleanup(t_settings *settings)
{
	int i;
	
	i = -1;
	while (++i < settings->philo_num)
		free(settings->eat_queue[i]);
	free(settings->eat_queue);
	free(settings->philos);
}

int main(int argc, char **argv)
{
	t_settings	settings;
	int				i;
	int				status;
	pid_t			pid;
	pid_t			wpid;

	if (argc < 5 || argc > 6 || !init(argc, argv, &settings))
	{
		printf("Error: Wrong arguements");
		return (0);
	}
	init_eat_rounds(&settings);
	pthread_mutex_init(&(settings.pen), NULL);
	i = -1;
	while (++i < settings.philo_num)
	{
		settings.philos[i].chair = i;
		settings.philos[i].eat_time = settings.eat_time;
		settings.philos[i].sleep_time = settings.sleep_time;
		settings.philos[i].settings = &settings;
		settings.philos[i].meals = 0;
		settings.philos[i].eat_round = 0;
		settings.philos[i].last_eaten = get_time();
		pid = fork();
		life(&(settings.philos[i]));
	}
	if (pid == 0)
		while ((wpid = wait(&status)) > 0);
	ft_cleanup(&settings);
}