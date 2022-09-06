#ifndef PHILO_H
# define PHILO_H
#include <stdbool.h>
#include <pthread.h>

typedef struct s_settings t_settings;

typedef struct s_philo
{
	int					chair;
	long				eat_time;
	long				last_eaten;
	long				sleep_time;
	int					holds_fork1;
	int					holds_fork2;
	int					meals;
	int					eat_round;
	pthread_mutex_t		fork; //need to destroy the mutex
	pthread_mutex_t		roundlock; //need to destroy the mutex
	pthread_t			death_tracker;
	t_settings			*settings;
}	t_philo;

typedef struct s_settings
{
	int args;
	int philo_num;
	int die_time;
	int eat_time;
	int sleep_time;
	int stop_after;
	int dead_count;
	int eat_rounds;
	int **eat_queue;
	int current_round;
	long start_time;
	pthread_mutex_t	pen; //TODO destroy mutex
	pthread_mutex_t	roundlock; //TODO destroy the mutex
	t_philo *philos;
}		t_settings;

// ft_utils.c
int			ft_atoi(const char *str);
int			ft_isnum(char *str);
long int	get_time(void);
void	pen(t_settings *settings, long timestamp, int philo_num, const char *action, int flag);
int		get_round(t_settings *settings, bool increase);

// philo_life.c
void    philo_eat(t_philo *philo, t_philo  *neighbour, int round);
void    monitor(long time, long action_time, t_philo *philo, const char *action);
void    *life(void *philo_arg);

// assign_eat.c
void	assign_two_eat_rounds(t_settings *settings);
void	assign_three_eat_rounds(t_settings *settings);
#endif
