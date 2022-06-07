#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include "philo.h"
#include <stdio.h>

int init(int argc, char **argv, t_settings *settings)
{
    int i;

    i = -1;
    while (++i < argc)
        if (!ft_isnum(argv[i]))
            return (0);
    settings->philos = ft_atoi(argv[0]);        
    settings->die_time = ft_atoi(argv[1]);
    settings->eat_time = ft_atoi(argv[2]);
    settings->sleep_time = ft_atoi(argv[3]);
    if (argc == 6)
        settings->stop_after = ft_atoi(argv[4]);
    return (1);
}

int main(int argc, char **argv)
{
    t_settings  settings;

    if (argc < 5 || argc > 6 || !init(argc, argv, &settings))
    {
        printf("Error: Wrong arguements");
        return (0);
    }
    // pthread_create(&newthread, NULL, myturn, NULL);
    printf("%s", argv[0]);
}