#include "philo.h"

void init_philos(t_params *params, t_philo *philos) {
    for (int i = 0; i < params->number_of_philosophers; i++) {
        philos[i].id = i + 1;
        philos[i].left_fork = &params->forks[i];
        philos[i].right_fork = &params->forks[(i + 1) % params->number_of_philosophers];
        philos[i].params = params;
        philos[i].last_meal_time = current_time();
        philos[i].meals_eaten = 0;
        pthread_mutex_init(&philos[i].last_meal_lock, NULL);
    }
	pthread_mutex_init(&params->lock_dead, NULL);
    params->start_time = current_time();
}

void print_status(t_philo *philo, const char *status) { // this log is slow and sloweverything
    t_params *params = philo->params;
    pthread_mutex_lock(&params->print_lock);
    printf("%lld %d %s\n", current_time() - params->start_time, philo->id, status);
    pthread_mutex_unlock(&params->print_lock);
}

void cleanup(t_params *params, t_philo *philos) {
    for (int i = 0; i < params->number_of_philosophers; i++)
        pthread_mutex_destroy(&params->forks[i]);
    pthread_mutex_destroy(&params->print_lock);
    free(params->forks);
}
/*
The philosophers alternatively eat, think, or sleep.
While they are eating, they are not thinking nor sleeping;
while thinking, they are not eating nor sleeping;
and, of course, while sleeping, they are not eating nor thinking.
*/
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
