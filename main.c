#include "philo.h"
#include <stdio.h>
void *supervising(void *arr)
{
	t_philo *philos;
	philos = (t_philo *) arr;
	t_params *params = philos[0].params;
	int i = 0;
	long long last_meal = 0;
	while(1)
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
			return NULL;
		}
		pthread_mutex_lock(&params->philos_done_lock);
		if (params->max_serving > 1 &&  params->philos_done >= params->num_philo)
		{
			params->dead = 1;
			printf("All philosophers have eaten %d times\n", params->max_serving);
			pthread_mutex_unlock(&params->philos_done_lock);
			return NULL;
		}
		pthread_mutex_unlock(&params->philos_done_lock);
		i ++;
	}
}
int main(int argc, char **argv) {
    if (argc != 5 && argc != 6) {
        printf("Usage: %s num_philo time_to_die time_to_eat time_to_sleep [max_serving]\n", argv[0]);
        return 1;
    }

    t_params params;
    params.num_philo = ft_atoi(argv[1]);
	if (params.num_philo < 1)
		return 0;
    params.time_to_die = ft_atoi(argv[2]);
    params.time_to_eat = ft_atoi(argv[3]);
    params.time_to_sleep = ft_atoi(argv[4]);
    params.max_serving = (argc == 6) ? ft_atoi(argv[5]) : -1;

    t_philo philos[params.num_philo];	
    params.forks = malloc(params.num_philo * sizeof(pthread_mutex_t));
	if (!params.forks)
		return 0;
    for (int i = 0; i < params.num_philo; i++)
        pthread_mutex_init(&params.forks[i], NULL);
    init_philos(&params, philos);
	pthread_t supervisor;
    for (int i = 0; i < params.num_philo; i++)
        pthread_create(&philos[i].thread, NULL, philosopher_thread, &philos[i]);

	pthread_create(&supervisor, NULL, supervising, &philos);
	pthread_join(supervisor, NULL);
    for (int i = 0; i < params.num_philo; i++)
    	pthread_join(philos[i].thread, NULL);
    cleanup(&params, philos);
	return 0;
}
