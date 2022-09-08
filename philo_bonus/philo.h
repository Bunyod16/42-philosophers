#ifndef PHILO_H
# define PHILO_H
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

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
	long start_time;
	sem_t fork_sets;
	sem_t	pen;
	t_philo *philos;
}		t_settings;

// ft_utils.c
int			ft_atoi(const char *str);
int			ft_isnum(char *str);
long int	get_time(void);
void	pen(t_settings *settings, long timestamp, int philo_num, const char *action, int flag);

// philo_life.c
void    philo_eat(t_philo *philo);
void    monitor(long time, long action_time, t_philo *philo, const char *action);
void    *life(t_philo *philo);

// assign_eat.c
void	assign_two_eat_rounds(t_settings *settings);
void	assign_three_eat_rounds(t_settings *settings);

// ft_check.c
int is_pos_int(char *str);
#endif
