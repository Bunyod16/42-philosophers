#ifndef PHILO_H
# define PHILO_H

typedef struct s_settings
{
	int philos;
	int die_time;
	int eat_time;
	int sleep_time;
	int stop_after;
}		t_settings;

int	ft_atoi(const char *str);
int ft_isnum(char *str);

#endif
