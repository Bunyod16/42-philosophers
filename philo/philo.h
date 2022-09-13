/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bunyodshams <bunyodshams@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 19:55:21 by bunyodshams       #+#    #+#             */
/*   Updated: 2022/09/13 10:53:57 by bunyodshams      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <stdbool.h>
# include <pthread.h>

typedef struct s_settings	t_settings;

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
	pthread_mutex_t		fork;
	pthread_mutex_t		roundlock;
	t_settings			*settings;
}	t_philo;

typedef struct s_settings
{
	int				args;
	int				philo_num;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	int				stop_after;
	int				dead_count;
	int				eat_rounds;
	int				**eat_queue;
	int				current_round;
	long			start_time;
	pthread_mutex_t	pen;
	pthread_mutex_t	roundlock;
	t_philo			*philos;
}		t_settings;

// ft_utils.c
int			ft_atoi(const char *str);
int			ft_isnum(char *str);
long int	get_time(void);
int			get_round(t_settings *settings, bool increase);
void		lock_forks(t_philo *p, t_philo *neighbour);

// philo_life.c
void		philo_eat(t_philo *philo, t_philo *neighbour, int round);
void		monitor(long action_time, t_philo *philo, const char *action);
void		*life(void *philo_arg);

// assign_eat.c
void		assign_two_eat_rounds(t_settings *settings);
void		assign_many_eat_rounds(t_settings *settings);
void		init_eat_rounds(t_settings *settings, int i);

// ft_check.c
void		pen(t_philo *philo, long timestamp, const char *action, int flag);
int			is_pos_int(char *str);
void		wait_all_philo_eat(t_philo *philo);
int			ate_last_round(t_philo *philo);
long		next_eat(t_philo *philo);

#endif
