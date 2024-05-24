#include "philo.h"

int	main(int argc, char **argv)
{
	t_params	params;
	int			i;
	t_philo		philos[200];
	pthread_t	supervisor;

	if (argc != 5 && argc != 6)
		return (1);
	params.num_philo = ft_atoi(argv[1]);
	if (params.num_philo < 1 || params.num_philo > 200)
		return (0);
	params.time_to_die = ft_atoi(argv[2]);
	params.time_to_eat = ft_atoi(argv[3]);
	params.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		params.max_serving = ft_atoi(argv[5]);
	else
		params.max_serving = -1;
	params.forks = malloc(params.num_philo * sizeof(pthread_mutex_t));
	if (!params.forks)
		return (0);
	i = 0;
	while (i < params.num_philo)
		pthread_mutex_init(&params.forks[i++], NULL);
	init_philos(&params, philos);
	i = 0;
	while (i < params.num_philo)
	{
		pthread_create(&philos[i].thread, NULL, philosopher_thread, &philos[i]);
		i++;
	}
	pthread_create(&supervisor, NULL, supervising, &philos);
	pthread_join(supervisor, NULL);
	i = 0;
	while (i < params.num_philo)
		pthread_join(philos[i++].thread, NULL);
	cleanup(&params, philos);
	return (0);
}
