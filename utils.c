#include "philo.h"

long long	current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		printf("something went wrong with getting the time\n");
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}
void	busy_wait(int microseconds)
{
	struct timeval	start;
	struct timeval	current;
	long long		elapsed;

	gettimeofday(&start, NULL);
	elapsed = 0;
	while (elapsed < microseconds)
	{
		usleep(100);
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000000LL + (current.tv_usec
				- start.tv_usec);
	}
}
void	*supervising(void *arr)
{
	t_philo *philos;
	philos = (t_philo *)arr;
	t_params *params = philos[0].params;
	int i = 0;
	long long last_meal = 0;
	while (1)
	{
		if (i >= params->num_philo)
		{
			busy_wait(1000);
			i = 0;
		}
		pthread_mutex_lock(&philos[i].last_meal_lock);
		last_meal = philos[i].last_meal_time;
		pthread_mutex_unlock(&philos[i].last_meal_lock);
		if ((last_meal + params->time_to_die) < current_time())
		{
			print_status(&philos[i], "died");
			pthread_mutex_lock(&params->lock_dead);
			params->dead = 1;
			pthread_mutex_unlock(&params->lock_dead);
			return (NULL);
		}
		pthread_mutex_lock(&params->philos_done_lock);
		if (params->max_serving > 1 && params->philos_done >= params->num_philo)
		{
			params->dead = 1;
			printf("All philosophers have eaten %d times\n",
				params->max_serving);
			pthread_mutex_unlock(&params->philos_done_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&params->philos_done_lock);
		i++;
	}
}