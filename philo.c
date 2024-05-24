#include "philo.h"

void init_philos(t_params *params, t_philo *philos) {
    for (int i = 0; i < params->num_philo; i++) {
        philos[i].id = i + 1;
        philos[i].left_fork = &params->forks[i];
        philos[i].right_fork = &params->forks[(i + 1) % params->num_philo];
        philos[i].params = params;
        philos[i].last_meal_time = current_time();
        philos[i].meals_eaten = 0;
        pthread_mutex_init(&philos[i].last_meal_lock, NULL);
    }
	pthread_mutex_init(&params->lock_dead, NULL);
	pthread_mutex_init(&params->print_lock, NULL);
	pthread_mutex_init(&params->philos_done_lock, NULL);
	params->philos_done = 0;
	params->dead = 0;
    params->start_time = current_time();
}

void print_status(t_philo *philo, const char *status) {
    t_params *params = philo->params;
    pthread_mutex_lock(&params->print_lock);
	pthread_mutex_lock(&params->lock_dead);
	if (params->dead)
	{
		pthread_mutex_unlock(&params->lock_dead);
		pthread_mutex_unlock(&params->print_lock);
		return;
	}
	pthread_mutex_unlock(&params->lock_dead);
	printf("%lld %d %s\n", current_time() - params->start_time, philo->id, status);
    pthread_mutex_unlock(&params->print_lock);
}

void cleanup(t_params *params, t_philo *philos) {
    for (int i = 0; i < params->num_philo; i++)
        pthread_mutex_destroy(&params->forks[i]);
    pthread_mutex_destroy(&params->print_lock);
    free(params->forks);
}

void *philosopher_thread(void *args)
{
	t_philo *philo = (t_philo *) args;
	if (philo->id % 2)
		busy_wait(philo->params->time_to_eat * 500);
	while(1)
	{
		if (philo->left_fork != philo->right_fork)
		{	
			pthread_mutex_lock(philo->left_fork);
			print_status(philo, "has taken a fork");
			pthread_mutex_lock(philo->right_fork);
			print_status(philo, "has taken a fork");
			print_status(philo, "is eating");
			pthread_mutex_lock(&philo->last_meal_lock);
			philo->last_meal_time = current_time();
			pthread_mutex_unlock(&philo->last_meal_lock);
			busy_wait(philo->params->time_to_eat * 1000);
			philo->meals_eaten ++;
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			if (philo->meals_eaten == philo->params->max_serving)
			{
				pthread_mutex_lock(&philo->params->philos_done_lock);
				philo->params->philos_done ++;
				pthread_mutex_unlock(&philo->params->philos_done_lock);
				//return NULL;
			}
			print_status(philo, "is sleeping");
			busy_wait(philo->params->time_to_sleep * 1000);
			print_status(philo, "is thinking");
		}
		pthread_mutex_lock(&philo->params->lock_dead);
		if (philo->params->dead)
		{
			pthread_mutex_unlock(&philo->params->lock_dead);
			return NULL;
		}
		pthread_mutex_unlock(&philo->params->lock_dead);
	}
	return NULL;
}
