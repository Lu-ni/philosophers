#include "philo.h"
#include <stdio.h>
void *supervising(void *arr)
{
	t_philo *philos;
	philos = (t_philo *) arr;
	int i = 0;
    printf("supervising !\n");
	while(1)
	{
		if (philos[i].id == -1)
			i = 0;
		pthread_mutex_lock(&philos[i].last_meal_lock);
		if ((philos[i].last_meal_time + 8000) < current_time()) //need to fetch time_do_die and replace 8000 value
		{
			printf("philo %i is dead because its last meal was at %lli and its now %lli\n", philos[i].id, philos[i].last_meal_time, current_time());
			exit(1);
	//		return NULL;
		}
		pthread_mutex_unlock(&philos[i].last_meal_lock);
		i ++;
	}
}
int main(int argc, char **argv) {
    if (argc != 5 && argc != 6) {
        printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n", argv[0]);
        return 1;
    }

    t_params params;
    params.number_of_philosophers = atoi(argv[1]);
    params.time_to_die = atoi(argv[2]);
    params.time_to_eat = atoi(argv[3]);
    params.time_to_sleep = atoi(argv[4]);
    params.number_of_times_each_philosopher_must_eat = (argc == 6) ? atoi(argv[5]) : -1;

    t_philo philos[params.number_of_philosophers + 1];
	philos[params.number_of_philosophers].id = -1;
    params.forks = malloc(params.number_of_philosophers * sizeof(pthread_mutex_t));
    for (int i = 0; i < params.number_of_philosophers; i++)
        pthread_mutex_init(&params.forks[i], NULL);

    init_philos(&params, philos);
	pthread_t supervisor;
	pthread_create(&supervisor, NULL, supervising, &philos);
    for (int i = 0; i < params.number_of_philosophers; i++)
        pthread_create(&philos[i].thread, NULL, philosopher_thread, &philos[i]);
	pthread_join(supervisor, NULL);

    for (int i = 0; i < params.number_of_philosophers; i++)
        pthread_join(philos[i].thread, NULL);

    cleanup(&params, philos);
    return 0;
}
