#include "philo.h"

int is_pos_int(char *str)
{
    if (!str)
        return (0);
    while (*str != '\0')
    {
        if (*str < '0' || *str > '9')
            return (0);
        str++;
    }
    return (1);
}