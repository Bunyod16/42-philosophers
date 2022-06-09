#include <limits.h>

int ft_isnum(char *str)
{
    int i;

    if (!str)
        return (0);
    i = -1;
    while (str[++i])
	    if (str[i] < '0' && str[i] > '9' )
            return (0);
    return (1);
}

int	ft_atoi(const char *str)
{
	int		sign;
	long	ret;

	ret = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		++str;
	if (*str == '+' || *str == '-')
		if (*(str++) == '-')
			sign *= -1;
	while (*str >= '0' && *str <= '9')
	{
		ret = ret * 10 + sign * (*str++ - '0');
		if (ret > INT_MAX)
			return (-1);
		else if (ret < INT_MIN)
			return (0);
	}
	return ((int)ret);
}

long int	get_time(void)
{
    struct timeval current_time;
    return (current_time.tv_sec * 1000 + current_time.tv_usec/1000);
}