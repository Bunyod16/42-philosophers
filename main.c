#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int init(int argc, char **argv, t_settings *settings)
{
	int i;

	i = -1;
	while (++i < argc)
		if (!ft_isnum(argv[i]))
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
	return (1);
}

long time_till_eat(t_philo *philo, int round)
{
	int i;

	i = 0;
	while (philo->settings->eat_queue[philo->chair][round] != 1)
	{
		round++;
		if (round == philo->settings->eat_rounds)
			round = 0;
		i++;
	}
	return (i * philo->settings->eat_time);
}

void    *life(void *philo_arg)
{
	t_philo     *philo;
	t_philo     *neighbour;
	int         i;
	int			round;

	philo = (t_philo *)philo_arg;
	i = philo->chair;
	if (philo->chair == philo->settings->philo_num - 1)
		i = philo->settings->philo_num;
	neighbour = &(philo->settings->philos[i + 1]); 
	while (1)
	{
		pthread_mutex_lock(&(philo->settings->roundlock));
		round = get_round(philo->settings, false);
		pthread_mutex_unlock(&(philo->settings->roundlock));
		if (philo->settings->eat_queue[philo->chair][round] == 1)
		{
			pthread_mutex_lock(&neighbour->fork);
			pen(philo->settings, get_time(), philo->chair, "has taken fork\n", 0);
			pthread_mutex_lock(&philo->fork);
			pen(philo->settings, get_time(), philo->chair, "has taken fork\n", 0);
			philo_eat(philo, neighbour, round);
			usleep(philo->settings->sleep_time);
		}
		else if (round == 0 
			|| (round > 0 
				&& philo->settings->eat_queue[philo->chair][round - 1] != 0))
		{
			monitor(philo->last_eaten, time_till_eat(philo, round), philo, "is thinking\n");
			pen(philo->settings, get_time(), philo->chair, "is thinking\n", 0);
		}
		while (usleep(100) == 0)
		{
			pthread_mutex_lock(&(philo->settings->roundlock));
			if (get_round(philo->settings, false) == round)
				break;
			pthread_mutex_unlock(&(philo->settings->roundlock));
		}
		printf("next round\n");
	}
	return (NULL);
}

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

void init_eat_rounds(t_settings *settings)
{
	int	eat_rounds;
	int	i;
	int eaters;
	int round;

	settings->eat_queue = malloc(sizeof(int *) * settings->philo_num + 1);
	// settings->eat_queue[settings->philo_num] = -1;
	eaters = settings->philo_num / 2;
	// predetermine how many eat rounds there are
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

int main(int argc, char **argv)
{
	t_settings	settings;
	int				i;
	pthread_t	*threads;

	if (argc < 5 || argc > 6 || !init(argc, argv, &settings))
	{
		printf("Error: Wrong arguements");
		return (0);
	}
	init_eat_rounds(&settings);
	i = -1;
	threads = malloc(sizeof(pthread_t) * settings.philo_num);
	pthread_mutex_init(&(settings.roundlock), NULL);
	settings.current_round = 0;
	while (++i < settings.philo_num)
	{
		settings.philos[i].chair = i;
		settings.philos[i].eat_time = settings.eat_time;
		settings.philos[i].sleep_time = settings.sleep_time;
		settings.philos[i].settings = &settings;
		settings.philos[i].last_eaten = get_time();
		pthread_mutex_init(&(settings.philos[i]).fork, NULL); //init fork
		pthread_mutex_init(&(settings.pen), NULL); //init fork
		pthread_create(&threads[i], NULL, life, (void *)&settings.philos[i]);
	}
	i = -1;
	while (++i < settings.philo_num)
	{
		pthread_join(threads[i],NULL);
		printf("joined thread %d\n",i);
	}
	printf("%s", argv[0]);
}