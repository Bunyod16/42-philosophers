#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "philo.h"
#include <stdio.h>
#include <stdlib.h>

int init(int argc, char **argv, t_settings *settings)
{
    int i;

    i = -1;
    while (++i < argc)
        if (!ft_isnum(argv[i]))
            return (0);
    settings->args = argc - 1;
    settings->philo_num = ft_atoi(argv[1]);        
    settings->die_time = ft_atoi(argv[2]);
    settings->eat_time = ft_atoi(argv[3]);
    settings->sleep_time = ft_atoi(argv[4]);
    settings->philos = malloc(sizeof(t_philo) * settings->philo_num);
    if (argc == 6)
        settings->stop_after = ft_atoi(argv[5]);
    return (1);
}

void    *life(void *philo_arg)
{
    t_philo     *philo;
    int         round;

    round = 0;
    philo = (t_philo *)philo_arg;
    while (1 && ++round)
    {
        pthread_mutex_lock(&philo->fork);
        printf("%ld #%d has taken a fork\n", get_time(), philo->chair);
        printf("%ld #%d is eating\n", get_time(), philo->chair);
        usleep(philo->eat_time);
        printf("%ld #%d is sleeping\n", get_time(), philo->chair);
        usleep(philo->sleep_time);
        printf("%ld #%d is thinking\n", get_time(), philo->chair);
        pthread_mutex_unlock(&philo->fork);
    }
    return (NULL);
}

int main(int argc, char **argv)
{
    t_settings  settings;
    int         i;
    pthread_t   *threads;

    if (argc < 5 || argc > 6 || !init(argc, argv, &settings))
    {
        printf("Error: Wrong arguements");
        return (0);
    }
    i = -1;
    threads = malloc(sizeof(pthread_t) * settings.philo_num);
    printf("%d num\n", settings.philo_num);
    while (++i < settings.philo_num)
    {
        settings.philos[i].chair = i + 1;
        settings.philos[i].eat_time = settings.eat_time;
        settings.philos[i].sleep_time = settings.sleep_time;
        pthread_mutex_init(&(settings.philos[i]).fork, NULL); //init fork
        pthread_create(&threads[i], NULL, life, (void *)&settings.philos[i]);
    }
    i = -1;
    printf("test");
    while (++i < settings.philo_num)
    {
        pthread_join(threads[i],NULL);
        printf("joined thread %d\n",i);
    }
    // start threads for each philo
    // each philo display time 
    // each philo check status of next fork
    printf("%s", argv[0]);
}